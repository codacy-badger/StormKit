#include <storm/engine/render/Context.hpp>
#include <storm/engine/render/Device.hpp>
#include <storm/engine/render/Surface.hpp>
#include <storm/log/LogOutput.hpp>
#include <storm/window/Window.hpp>

template<typename T>
static constexpr const T WINDOW_WIDTH = 800;

template<typename T>
static constexpr const T WINDOW_HEIGHT = 600;

struct Vertex {
    storm::engine::vec2 position;
    storm::engine::vec3 color;
};

static const auto VERTICES = std::array<Vertex, 6>{
  Vertex{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}},
  Vertex{{0.5f, 0.5f}, {0.f, 1.f, 0.f}},
  Vertex{{-0.5f, 0.5f}, {0.f, 0.f, 1.f}},
  Vertex{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}},
  Vertex{{0.5f, 0.5f}, {0.f, 1.f, 0.f}},
  Vertex{{0.5f, -0.5f}, {0.f, 0.f, 1.f}},
};

struct Matrices {
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
};

void runApp();

int main() {
    storm::InstallDefaultLogOutput();

    try {
        runApp();
    } catch(std::exception &e) {
        storm::FLOG("Exception Not Handled ! : %{1}", e.what());

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void runApp() {
    namespace core   = storm::core;
    namespace window = storm::window;
    namespace engine = storm::engine;

    using Clock = std::chrono::high_resolution_clock;

    auto settings              = engine::ContextSettings{};
    settings.enable_validation = true;
    settings.app_name          = "StormKit Uniform buffer Example";
    settings.app_version.minor = 1;
    settings.app_version.major = 0;

    auto render_window =
      window::Window{"StormKit Uniform buffer Example",
                     {{WINDOW_WIDTH<std::uint16_t>, WINDOW_HEIGHT<std::uint16_t>}, 32u, 2u},
                     window::WindowStyle::Close};

    auto context = engine::Context{settings};
    auto device =
      engine::Device{context, render_window.nativeHandle(), context.bestPhysicalDevice()};
    auto surface = engine::Surface{device, render_window};

    auto vertex_shader =
      device.createShader(engine::Shader::Stage::VERTEX, "shaders/uniformbuffer_spv.vert");
    auto fragment_shader =
      device.createShader(engine::Shader::Stage::FRAGMENT, "shaders/uniformbuffer_spv.frag");

    auto program = device.createProgram();
    program.addShaderModule(vertex_shader);
    program.addShaderModule(fragment_shader);
    program.link();

    auto matrices = Matrices{};
    matrices.projection =
      glm::perspective(glm::radians(45.f), WINDOW_WIDTH<float> / WINDOW_HEIGHT<float>, 0.1f, 10.f);

    matrices.projection[ 1 ][ 1 ] *= -1;
    matrices.view  = glm::lookAt(glm::vec3{2.f, 2.f, 2.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f});
    matrices.model = glm::mat4{1.f};

    const auto vertex_buffer_desc = engine::HardwareBuffer::Description{
      std::size(VERTICES) * sizeof(Vertex), alignof(Vertex), engine::BufferUsageFlag::VERTEX};

    auto vertex_buffer = device.createHardwareBuffer(std::move(vertex_buffer_desc));
    vertex_buffer.setData(VERTICES);

    auto uniform_buffer_description = engine::HardwareBuffer::Description{
      sizeof(Matrices), alignof(Matrices), engine::BufferUsageFlag::UNIFORM};

    auto uniform_buffer = device.createHardwareBuffer(std::move(uniform_buffer_description));
    uniform_buffer.setData(reinterpret_cast<std::byte *>(&matrices), sizeof(Matrices), 0u);

    auto render_pass = device.createRenderPass(true);
    auto framebuffer = device.createFramebuffer();
    framebuffer.setExtent({WINDOW_WIDTH<std::uint32_t>, WINDOW_HEIGHT<std::uint32_t>, 1u});
    framebuffer.addOutputAttachment(
      {engine::ColorFormat::RGBA8888UNORM,
       {WINDOW_WIDTH<std::uint32_t>, WINDOW_HEIGHT<std::uint32_t>, 1u},
       1u});

    render_pass.setFramebuffer(framebuffer);
    render_pass.build();

    auto command_buffer = device.createCommandBuffer();
    command_buffer.pipelineState().viewport =
      engine::Viewport{0, 0, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>};
    command_buffer.pipelineState().scissor =
      engine::vec4{0, 0, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>};
    command_buffer.pipelineState().vertex_input_state.bindings = {
      {0, sizeof(Vertex), engine::VertexInputRate::PER_VERTEX}};
    command_buffer.pipelineState().vertex_input_state.attributes = {
      {0, engine::Format::FLOAT_VEC2, 0, offsetof(Vertex, position)},
      {0, engine::Format::FLOAT_VEC3, 1, offsetof(Vertex, color)}};
    command_buffer.pipelineState().cull_mode = engine::CullMode::NONE;

    command_buffer.bindingState().bindings = {engine::UniformBufferBinding{
      &uniform_buffer, 0, sizeof(Matrices), 0, engine::Shader::Stage::VERTEX}};

    command_buffer.begin();
    command_buffer.beginRenderPass(render_pass, framebuffer);
    command_buffer.setProgram(program);
    command_buffer.bindVertexBuffer(0, vertex_buffer);
    command_buffer.draw(std::size(VERTICES));
    command_buffer.endRenderPass();
    command_buffer.end();

    auto last            = Clock::now();
    auto last_fps_update = Clock::now();
    auto start_time      = Clock::now();
    while(render_window.isOpen()) {
        auto now   = Clock::now();
        auto delta = std::chrono::duration<float, std::chrono::seconds::period>{now - last}.count();

        if(std::chrono::duration_cast<std::chrono::seconds>(now - last_fps_update).count() >= 1.f) {
            if(delta == 0.f) delta = 1.f;
            render_window.setTitle(core::format(
              "StormKit Uniform Buffer Example | Delta : %{1} | FPS : %{2}", delta, 1.f / delta));
            last_fps_update = now;
        }

        last = now;

        auto event = window::Event{};

        while(render_window.pollEvent(event)) {
            if(event.type == window::EventType::Closed) {
                render_window.close();
                break;
            }
        }

        const auto time =
          std::chrono::duration<float, std::chrono::seconds::period>{now - start_time}.count();

        matrices.model =
          glm::rotate(glm::mat4{1.f}, time * glm::radians(90.0f), glm::vec3{0.0f, 0.0f, 1.0f});
        uniform_buffer.setData(
          reinterpret_cast<std::byte *>(&matrices.model),
          sizeof(glm::mat4),
          offsetof(Matrices, model));

        auto frame = surface.nextFrame();

        command_buffer.submit({}, {&frame.render_finished});

        surface.present(framebuffer, frame);

        render_window.display();
    }

    device.waitIdle();
}
