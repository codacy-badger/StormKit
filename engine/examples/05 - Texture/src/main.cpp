#include <storm/log/LogOutput.hpp>

#include <storm/window/Window.hpp>

#include <storm/engine/render/Context.hpp>
#include <storm/engine/render/Device.hpp>
#include <storm/engine/render/Surface.hpp>

#include <storm/image/Image.hpp>

template <typename T>
static constexpr const T WINDOW_WIDTH = 800;

template <typename T>
static constexpr const T WINDOW_HEIGHT = 600;

struct Vertex {
	storm::engine::vec2 position;
	storm::engine::vec3 color;
	storm::engine::vec2 uv;
};

static const auto VERTICES = std::array<Vertex, 6> {
	Vertex{{0.f,   0.f},   {1.f, 1.f, 1.f}, {0.f, 0.f}},
	Vertex{{300.f, 0.f},   {1.f, 1.f, 1.f}, {1.f, 0.f}},
	Vertex{{300.f, 378.f}, {1.f, 1.f, 1.f}, {1.f, 1.f}},
	Vertex{{0.f,   0.f},   {1.f, 1.f, 1.f}, {0.f, 0.f}},
	Vertex{{300.f, 378.f}, {1.f, 1.f, 1.f}, {1.f, 1.f}},
	Vertex{{0.f,   378.f}, {1.f, 1.f, 1.f}, {0.f, 1.f}},
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
	} catch (std::exception &e) {
		storm::FLOG("Exception Not Handled ! : %{1}", e.what());
		
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

void runApp() {
	namespace core   = storm::core;
	namespace window = storm::window;
	namespace engine = storm::engine;
	namespace image  = storm::image;
	
	auto settings = engine::ContextSettings{};
	settings.enable_validation = true;
	settings.app_name = "StormKit Texture Example";
	settings.app_version.minor = 1;
	settings.app_version.major = 0;	
	
	auto render_window = window::Window{"StormKit Texture Example",
							{{WINDOW_WIDTH<std::uint16_t>, WINDOW_HEIGHT<std::uint16_t>}, 32u, 2u},
							window::WindowStyle::Close};
		
	auto context = engine::Context{settings};
	auto device  = engine::Device{context, render_window.nativeHandle(), context.bestPhysicalDevice()};
	auto surface = engine::Surface{device, render_window};

	auto vertex_shader = device.createShader(engine::Shader::Stage::VERTEX, "shaders/texture_spv.vert");
	auto fragment_shader = device.createShader(engine::Shader::Stage::FRAGMENT, "shaders/texture_spv.frag");

	auto program = device.createProgram();
	program.addShaderModule(vertex_shader);
	program.addShaderModule(fragment_shader);
	program.link();

	auto vertex_buffer = device.createVertexBuffer(std::size(VERTICES) * sizeof(Vertex));
	vertex_buffer.addData(VERTICES);
	
	auto matrices = Matrices{};
	matrices.projection = glm::ortho(0.f, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>, 0.f);
	matrices.view = glm::mat4{1.f};
	matrices.model = glm::translate(glm::mat4{1.f}, {
		WINDOW_WIDTH<float> / 2.f - 150.f,
		WINDOW_HEIGHT<float> / 2.f - 189.f,
		0.f
	});

	auto uniform_buffer = device.createUniformBuffer(sizeof(Matrices));
	uniform_buffer.addData(reinterpret_cast<std::byte*>(&matrices), sizeof(Matrices));
	
	auto image_file = image::Image{};
	image_file.loadFromFile("textures/texture.png");
	
	auto texture = device.createTexture(image_file);

	auto render_pass = device.createRenderPass(true);
	render_pass.addAttachment(engine::ColorFormat::RGBA8888UNORM);
	render_pass.setExtent({WINDOW_WIDTH<std::uint32_t>, WINDOW_HEIGHT<std::uint32_t>});

	auto command_buffer = device.createCommandBuffer();
	command_buffer.pipelineState().viewport = engine::Viewport{0, 0, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>};
	command_buffer.pipelineState().scissor = engine::vec4{0, 0, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>};
	
	command_buffer.pipelineState().vertex_input_state.bindings = {
		{0, sizeof(Vertex), engine::VertexInputRate::PER_VERTEX}
	};
	command_buffer.pipelineState().vertex_input_state.attributes = {
		{0, engine::Format::FLOAT_VEC2, 0, offsetof(Vertex, position)},
		{0, engine::Format::FLOAT_VEC3, 1, offsetof(Vertex, color)},
		{0, engine::Format::FLOAT_VEC2, 2, offsetof(Vertex, uv)}
	};
	command_buffer.pipelineState().cull_mode = engine::CullMode::NONE;
	
	command_buffer.bindingState().bindings = {
		engine::UniformBufferBinding{&uniform_buffer, 0, sizeof(Matrices), 0, engine::Shader::Stage::VERTEX},
		engine::TextureBinding{&texture, 1}
	};

	command_buffer.begin();
	command_buffer.beginRenderPass(render_pass);
	command_buffer.setProgram(program);
	command_buffer.bindVertexBuffer(0, vertex_buffer);
	command_buffer.draw(std::size(VERTICES));
	command_buffer.endRenderPass();
	command_buffer.end();

	auto render_fence     = device.createFence();
	auto render_semaphore = device.createSemaphore();

	while(render_window.isOpen()) {
		auto event = window::Event{};
		
		while(render_window.pollEvent(event)) {
			if(event.type == window::EventType::Closed) {
				render_window.close();
				break;
			}
		}

		render_fence.wait();
		render_fence.reset();

		command_buffer.submit({}, { &render_semaphore });

		surface.presentFrame(render_pass.framebuffer(), render_semaphore, render_fence);
		
		render_window.display();
	}

	device.waitIdle();
}

