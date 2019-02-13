#include <storm/engine/render/Context.hpp>
#include <storm/engine/render/Device.hpp>
#include <storm/engine/render/Surface.hpp>
#include <storm/log/LogOutput.hpp>
#include <storm/window/Window.hpp>

template <typename T>
static constexpr const T WINDOW_WIDTH = 800;

template <typename T>
static constexpr const T WINDOW_HEIGHT = 600;

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

	auto settings              = engine::ContextSettings {};
	settings.enable_validation = true;
	settings.app_name          = "StormKit Triangle Example";
	settings.app_version.minor = 1;
	settings.app_version.major = 0;

	auto render_window = window::Window {"StormKit Triangle Example",
	    {{WINDOW_WIDTH<std::uint16_t>, WINDOW_HEIGHT<std::uint16_t>}, 32u, 2u},
	    window::WindowStyle::Close};

	auto context = engine::Context {settings};
	auto device  = engine::Device {
        context, render_window.nativeHandle(), context.bestPhysicalDevice()};
	auto surface = engine::Surface {device, render_window};

	auto vertex_shader = device.createShader(
	    engine::Shader::Stage::VERTEX, "shaders/triangle_spv.vert");
	auto fragment_shader = device.createShader(
	    engine::Shader::Stage::FRAGMENT, "shaders/triangle_spv.frag");

	auto program = device.createProgram();
	program.addShaderModule(vertex_shader);
	program.addShaderModule(fragment_shader);
	program.link();

	auto render_pass = device.createRenderPass(true);
	render_pass.addAttachment(engine::ColorFormat::RGBA8888UNORM);
	render_pass.setExtent(
	    {WINDOW_WIDTH<std::uint32_t>, WINDOW_HEIGHT<std::uint32_t>});

	auto command_buffer = device.createCommandBuffer();
	command_buffer.pipelineState().viewport
	    = engine::Viewport {0, 0, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>};
	command_buffer.pipelineState().scissor
	    = engine::vec4 {0, 0, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>};
	command_buffer.pipelineState().cull_mode = engine::CullMode::NONE;
	command_buffer.begin();
	command_buffer.beginRenderPass(render_pass);
	command_buffer.setProgram(program);
	command_buffer.draw(3);
	command_buffer.endRenderPass();
	command_buffer.end();

	auto render_fence     = device.createFence();
	auto render_semaphore = device.createSemaphore();

	while (render_window.isOpen()) {
		auto event = window::Event {};

		while (render_window.pollEvent(event)) {
			if (event.type == window::EventType::Closed) {
				render_window.close();
				break;
			}
		}

		render_fence.wait();
		render_fence.reset();

		command_buffer.submit({}, {&render_semaphore});

		surface.presentFrame(
		    render_pass.framebuffer(), render_semaphore, render_fence);

		render_window.display();
	}

	device.waitIdle();
}
