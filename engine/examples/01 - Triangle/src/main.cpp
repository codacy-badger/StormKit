#include <storm/engine/render/Context.hpp>
#include <storm/engine/render/Device.hpp>
#include <storm/engine/render/Surface.hpp>
#include <storm/log/LogOutput.hpp>
#include <storm/window/Window.hpp>

#include <chrono>

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

	using Clock = std::chrono::high_resolution_clock;

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
	auto framebuffer = device.createFramebuffer();
	framebuffer.setExtent({WINDOW_WIDTH<std::uint32_t>, WINDOW_HEIGHT<std::uint32_t>, 1u});
    framebuffer.addOutputAttachment({engine::ColorFormat::RGBA8888UNORM, {WINDOW_WIDTH<std::uint32_t>, WINDOW_HEIGHT<std::uint32_t>, 1u}, 1u});

	render_pass.setFramebuffer(framebuffer);
	render_pass.build();

	auto command_buffer = device.createCommandBuffer();
	command_buffer.pipelineState().viewport
		= engine::Viewport {0, 0, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>};
	command_buffer.pipelineState().scissor
		= engine::vec4 {0, 0, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>};
	command_buffer.pipelineState().cull_mode = engine::CullMode::NONE;
	command_buffer.begin();
	command_buffer.beginRenderPass(render_pass, framebuffer);
	command_buffer.setProgram(program);
	command_buffer.draw(3);
	command_buffer.endRenderPass();
	command_buffer.end();

	auto last = Clock::now();
	auto last_fps_update = Clock::now();
	while (render_window.isOpen()) {
		auto now = Clock::now();

		if(std::chrono::duration_cast<std::chrono::seconds>(now - last_fps_update).count() >= 1.f) {
			auto delta = std::chrono::duration<float, std::chrono::seconds::period>{now - last}.count();
			if(delta == 0) delta = 1;
			render_window.setTitle(core::format("StormKit Triangle Example | Delta : %{1} | FPS : %{2}", delta, 1.f / delta));
			last_fps_update = now;
		}

		last = now;

		auto event = window::Event {};

		while (render_window.pollEvent(event)) {
			if (event.type == window::EventType::Closed) {
				render_window.close();
				break;
			}
		}

		auto frame = surface.nextFrame();

		command_buffer.submit(
			{},
			{&frame.render_finished}
		);

		surface.present(framebuffer, frame);

		render_window.display();
	}

	device.waitIdle();
}
