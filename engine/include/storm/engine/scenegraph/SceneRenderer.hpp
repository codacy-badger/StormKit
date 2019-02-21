// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <functional>
#include <storm/core/Filesystem.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/engine/graphics/RenderGraph.hpp>
#include <storm/engine/graphics/ShaderLibrary.hpp>
#include <storm/engine/render/Fence.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Framebuffer.hpp>
#include <storm/engine/render/HardwareBuffer.hpp>
#include <storm/engine/render/Program.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/render/Semaphore.hpp>
#include <storm/engine/render/Shader.hpp>
#include <storm/engine/render/Types.hpp>
#include <storm/engine/scenegraph/ForwardDeclarations.hpp>

namespace storm::engine {
    class SceneRenderer : public core::NonCopyable {
    public:
        explicit SceneRenderer(const Device &device, const Surface &surface, uvec2 render_extent);
        ~SceneRenderer();

        SceneRenderer(SceneRenderer &&);
        SceneRenderer &operator=(SceneRenderer &&);

        void render(Scene &scene);

        void exportRenderGraph(const _std::filesystem::path &filepath) const;

    private:
        struct MeshData {
            mat4 transform;
        };

        struct CameraData {
            mat4 projection = mat4{1.f};
            mat4 view       = mat4{1.f};
        } m_camera;

        std::reference_wrapper<const Device> m_device;
        std::reference_wrapper<const Surface> m_surface;
        uvec2 m_render_extent;

        RenderGraph m_render_graph;
        ShaderLibrary m_shader_library;

        Shader::Ptr m_forward_vert;
        Shader::Ptr m_forward_frag;
        Program::Ptr m_forward_program;

        Fence m_fence;
        Semaphore m_semaphore;
        std::vector<CommandBuffer> m_command_buffers;
        std::uint32_t m_current_command_buffer;

        HardwareBuffer::Description m_camera_buffer_desc;
        HardwareBuffer m_camera_buffer;

        HardwareBuffer::Description m_mesh_data_buffer_desc;
        HardwareBuffer m_mesh_data_buffer;

        std::unordered_map<std::string, RenderPass::Ptr> m_render_passes;
        Framebuffer::Ptr m_backbuffer;
    };
} // namespace storm::engine
