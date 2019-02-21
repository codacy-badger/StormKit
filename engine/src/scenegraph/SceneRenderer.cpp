// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "DefaultRenderTaskData.hpp"

#include <storm/engine/render/Device.hpp>
#include <storm/engine/render/Surface.hpp>
#include <storm/engine/scenegraph/Scene.hpp>
#include <storm/engine/scenegraph/SceneRenderer.hpp>

using namespace storm::engine;

static constexpr const auto DEFAULT_MESH_DATA_BUFFER_SIZE = 100 * sizeof(mat4);

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer::SceneRenderer(const Device &device, const Surface &surface, uvec2 render_extent)
      : m_device{device}, m_surface{surface}, m_render_extent{render_extent}, m_fence{m_device},
        m_semaphore{m_device}, m_camera_buffer_desc{sizeof(CameraData),
                                                    alignof(CameraData),
                                                    BufferUsageFlag::UNIFORM},
        m_camera_buffer{m_device, m_camera_buffer_desc},
        m_mesh_data_buffer_desc{DEFAULT_MESH_DATA_BUFFER_SIZE,
                                alignof(MeshData),
                                BufferUsageFlag::UNIFORM},
        m_mesh_data_buffer{m_device, m_mesh_data_buffer_desc} {

    auto camera = CameraData{};

    m_camera_buffer.setData(reinterpret_cast<const std::byte *>(&camera), sizeof(camera), 0u);

    const auto &vert_data = m_shader_library.getSource(ShaderLibrary::FORWARD_RENDER_VERT);
    const auto &frag_data = m_shader_library.getSource(ShaderLibrary::FORWARD_RENDER_FRAG);
    m_forward_vert        = m_device.get().createShaderPtr(
      engine::Shader::Stage::VERTEX, std::data(vert_data), std::size(vert_data));
    m_forward_frag = m_device.get().createShaderPtr(
      engine::Shader::Stage::FRAGMENT, std::data(frag_data), std::size(frag_data));

    m_forward_program = m_device.get().createProgramPtr();

    m_forward_program->addShaderModule(*m_forward_vert);
    m_forward_program->addShaderModule(*m_forward_frag);
    m_forward_program->link();
}

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer::~SceneRenderer() = default;

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer::SceneRenderer(SceneRenderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer &SceneRenderer::operator=(SceneRenderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::render(Scene &scene) {
}

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::exportRenderGraph(const _std::filesystem::path &filepath) const {
    // m_render_graph.exportGraphviz(filepath);
}
