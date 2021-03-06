#include <storm/log/LogOutput.hpp>

#include <storm/window/Window.hpp>

#include <storm/engine/render/Context.hpp>
#include <storm/engine/render/Device.hpp>
#include <storm/engine/render/Surface.hpp>

template <typename T>
static constexpr const T WINDOW_WIDTH = 800;

template <typename T>
static constexpr const T WINDOW_HEIGHT = 600;

struct Vertex {
	storm::engine::vec2 position;
	storm::engine::vec3 color;
};

static const auto VERTICES = std::array<Vertex, 4> {
	Vertex{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}},
	Vertex{{0.5f,  0.5f},  {0.f, 1.f, 0.f}},
	Vertex{{-0.5f,  0.5f}, {0.f, 0.f, 1.f}},
	Vertex{{0.5f,  -0.5f}, {0.f, 0.f, 1.f}},
};

static const auto INDICES = std::array<std::uint16_t, 6> {
	0, 1, 2, 3, 1, 0
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
	
	auto settings = engine::ContextSettings{};
	settings.enable_validation = true;
	settings.app_name = "StormKit Index buffer Example";
	settings.app_version.minor = 1;
	settings.app_version.major = 0;	
	
	auto render_window = window::Window{"StormKit Index buffer Example",
							{{WINDOW_WIDTH<std::uint16_t>, WINDOW_HEIGHT<std::uint16_t>}, 32u, 2u},
							window::WindowStyle::Close};
		
	auto context = engine::Context{settings};
	auto device  = engine::Device{context, render_window.nativeHandle(), context.bestPhysicalDevice()};
	auto surface = engine::Surface{device, render_window};

	auto vertex_shader = device.createShader(engine::Shader::Stage::VERTEX, "shaders/indexbuffer_spv.vert");
	auto fragment_shader = device.createShader(engine::Shader::Stage::FRAGMENT, "shaders/indexbuffer_spv.frag");

	auto program = device.createProgram();
	program.addShaderModule(vertex_shader);
	program.addShaderModule(fragment_shader);
	program.link();

	auto vertex_buffer = device.createVertexBuffer(std::size(VERTICES) * sizeof(Vertex));
	auto index_buffer = device.createIndexBuffer(std::size(INDICES) * sizeof(Vertex));
	vertex_buffer.addData(VERTICES);
	index_buffer.addData(INDICES);

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
		{0, engine::Format::FLOAT_VEC3, 1, offsetof(Vertex, color)}
	};
	command_buffer.pipelineState().cull_mode = engine::CullMode::NONE;

	command_buffer.begin();
	command_buffer.beginRenderPass(render_pass);
	command_buffer.setProgram(program);
	command_buffer.bindVertexBuffer(0, vertex_buffer);
	command_buffer.bindIndexBuffer(index_buffer);
	command_buffer.drawIndexed(std::size(INDICES));
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





/*#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <storm/log/LogOutput.hpp>

#include <storm/engine/render/RenderWindow.hpp>
#include <storm/engine/render/RenderSurface.hpp>
#include <storm/engine/render/PhysicalDevice.hpp>
#include <storm/engine/render/Pipeline.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/render/Shader.hpp>
#include <storm/engine/render/Program.hpp>
#include <storm/engine/render/CommandBuffer.hpp>
#include <storm/engine/render/HardwareBuffer.hpp>
#include <storm/engine/render/Renderer.hpp>
#include <storm/engine/render/Texture.hpp>

#include <storm/engine/graphics/Vertex.hpp>

#include <storm/image/Image.hpp>

//Constantes
static constexpr const auto WINDOW_WIDTH  = 800;
static constexpr const auto WINDOW_HEIGHT = 600;
static constexpr const auto PROGRAM_NAME  = "hello_cube";
static const auto PROJECTION_MATRIX       = glm::perspective(glm::radians(45.f),
															 static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
															 0.1f, 10.f);
static const auto VIEW_MATRIX = glm::lookAt(glm::vec3{2.f, 2.f, 2.f},
{0.f, 0.f, 0.f},
{0.f, 0.f, 1.f});
static constexpr const auto ROTATE_FACTOR = 1.f / 100000.f;

int main() {
	namespace window = storm::window;
	namespace engine = storm::engine;
	namespace core   = storm::core;
	namespace image  = storm::image;
	
	//initialisation des loggers
	stormInstallDefaultLogOutput();
	
	engine::AppSettings settings;
	settings.enable_validation = true;
	settings.app_name = PROGRAM_NAME;
	settings.app_version.minor = 1;
	settings.app_version.major = 0;
	
	//Creation et configuration de la fenetre
	auto render_window = engine::RenderWindow{"Triangle !", {WINDOW_WIDTH, WINDOW_HEIGHT, 32}, window::WindowStyle::Fullscreen | window::WindowStyle::Close, settings};
	auto renderer      = engine::Renderer{render_window.device(), render_window.swapchain()};
	renderer.initDefaultPipelines({WINDOW_WIDTH, WINDOW_HEIGHT});
	
	auto &pipeline_data = renderer.pipelineData(renderer.default3DPipeline());
	pipeline_data.projection = PROJECTION_MATRIX;
	pipeline_data.view = VIEW_MATRIX;
	
	//Creation des shaders et du program
	const auto &device = render_window.device();
	
	static const auto vertices = std::array<engine::Vertex_P_C, 8>{
		engine::Vertex_P_C{{ 0.25f, 0.f, 0.f},  {1.f, 0.f, 0.f, 1.f}},
						  {{ 0.5f,  0.f, 0.5f},     {0.f, 1.f, 0.f, 1.f}},
						  {{ 0.f,   0.f, 0.5f},     {0.f, 0.f, 1.f, 1.f}},
						  {{ 0.25f, -0.125f, 0.f},  {1.f, 0.f, 0.f, 1.f}},
						  {{ 0.5f,  -0.125f, 0.5f}, {0.f, 1.f, 0.f, 1.f}},
						  {{ 0.f,   -0.125f, 0.5f}, {0.f, 0.f, 1.f, 1.f}},
	};
	
	auto vertex_buffer = renderer.allocateBuffer(engine::BufferUsage::VERTEX_BIT, std::size(vertices) * sizeof(engine::Vertex_P_C));
	auto vertex_buffer_data = vertex_buffer->map();
	std::copy(std::begin(vertices), std::end(vertices), reinterpret_cast<engine::Vertex_P_C*>(vertex_buffer_data));
	vertex_buffer->unmap();
	
	static const auto indices = std::array<std::uint16_t, 18> {
		0, 1, 2,
		0, 1, 4,
		4, 1, 5,
		3, 2, 5,
		3, 4, 5,
		3, 0, 2
	};
	
	auto index_buffer = renderer.allocateBuffer(engine::BufferUsage::INDEX_BIT, std::size(indices) * sizeof(std::uint16_t));
	auto index_buffer_data = index_buffer->map();
	std::copy(std::begin(indices), std::end(indices), reinterpret_cast<std::uint16_t*>(index_buffer_data));
	index_buffer->unmap();
	
	auto drawable = engine::Drawable{};
	drawable.command_buffer = renderer.createCommandBuffer();
	drawable.command_buffer->addCommand(0, engine::Command::DrawIndexed::makeUnique(0, std::size(indices), *vertex_buffer, *index_buffer));
	drawable.command_buffer->build(renderer.default3DPipeline());
	drawable.model_matrix = glm::rotate(glm::mat4{1.f}, glm::radians(90.f), glm::vec3{0.f, 0.f, 1.f});
			
	renderer.addDrawable(renderer.default3DPipeline(), drawable);

	static const auto square_vertices = std::array<engine::Vertex_P_C_UV, 4>{
		engine::Vertex_P_C_UV{{ -1.f, -1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
							 {{ 1.f,  -1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 0.f}},
							 {{ -1.f, 1.f,  0.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 1.f}},
							 {{ 1.f,   1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f}},
	};
	
	auto square_vertex_buffer = renderer.allocateBuffer(engine::BufferUsage::VERTEX_BIT, std::size(square_vertices) * sizeof(engine::Vertex_P_C_UV));
	auto square_vertex_buffer_data = square_vertex_buffer->map();
	std::copy(std::begin(square_vertices), std::end(square_vertices), reinterpret_cast<engine::Vertex_P_C_UV*>(square_vertex_buffer_data));
	square_vertex_buffer->unmap();
	
	static const auto square_indices = std::array<std::uint16_t, 6> {
		0, 1, 2, 2, 3, 1
	};
	
	auto square_index_buffer = renderer.allocateBuffer(engine::BufferUsage::INDEX_BIT, std::size(square_indices) * sizeof(std::uint16_t));
	auto square_index_buffer_data = square_index_buffer->map();
	std::copy(std::begin(square_indices), std::end(square_indices), reinterpret_cast<std::uint16_t*>(square_index_buffer_data));
	square_index_buffer->unmap();
	
	auto texture_image = image::Image{};
	texture_image.loadFromFile("images/image.png", image::Image::Codec::PNG);
	auto texture_image_f = image::Image::flipY(texture_image);
	
	auto texture = engine::Texture{texture_image_f, render_window.device()};
	
	auto square_drawable = engine::Drawable{};
	square_drawable.command_buffer = renderer.createCommandBuffer();
	square_drawable.command_buffer->addCommand(0, engine::Command::DrawIndexed::makeUnique(
												  0,
												  std::size(square_indices), 
												  *square_vertex_buffer,
												  *square_index_buffer
												));
	square_drawable.command_buffer->setTextureBindings(engine::TextureBinding{1, &texture});
	square_drawable.command_buffer->build(renderer.default2DPipeline());
	square_drawable.model_matrix = engine::mat4{1.f};
	
	//renderer.addDrawable(renderer.default2DPipeline(), square_drawable);
	
	//Rendu
	auto start_time = std::chrono::high_resolution_clock::now();
	auto last_time = start_time;
	auto last_fps_time = start_time;
	while(render_window.isOpen()) {
		window::Event event;
		while(render_window.pollEvent(event)) {
			if(event.type == window::EventType::Closed || (event.type == window::EventType::KeyPressed && event.keyEvent.key == window::Key::ESCAPE)) {
				render_window.close();
			}
		}
		
		auto current_time = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
		
		if(std::chrono::duration_cast<std::chrono::seconds>(current_time - last_fps_time).count() >= 1.f) {
			const auto delta = std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_time).count();
			const auto fps = 1000000.f / delta;
			
			render_window.setTitle(storm::core::format("Triangle | fps: %{1}", fps));
			
			last_fps_time = current_time;
		}
		
		last_time = current_time;
		
		drawable.model_matrix = glm::rotate(glm::mat4{1.f}, time * glm::radians(90.f), glm::vec3{1.f, 0.f, 0.f});
		
		renderer.render();
		
		render_window.display();
	}
	
	return EXIT_SUCCESS;
}
*/
