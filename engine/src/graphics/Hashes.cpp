// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/core/Numerics.hpp>
#include <storm/core/TypeTraits.hpp>
#include <storm/core/Types.hpp>
#include <storm/engine/graphics/Hashes.hpp>
#include <storm/engine/graphics/Vertex.hpp>
#include <storm/engine/render/BindingState.hpp>
#include <storm/engine/render/Hashes.hpp>
#include <storm/engine/render/Shader.hpp>

using namespace storm;
using namespace storm::core;
using namespace storm::engine;

static auto uint32_t_hasher  = std::hash<std::uint32_t>{};
static auto uint64_t_hasher  = std::hash<std::uint64_t>{};
static auto ptrdiff_t_hasher = std::hash<std::ptrdiff_t>{};
static auto size_t_hasher    = std::hash<std::size_t>{};
static auto float_hasher     = std::hash<float>{};
static auto vec2_hasher      = std::hash<vec2>{};
static auto vec3_hasher      = std::hash<vec3>{};
static auto vec4_hasher      = std::hash<vec4>{};
static auto mat4_hasher      = std::hash<mat4>{};
static auto quat_hasher      = std::hash<quat>{};
static auto color_hasher     = std::hash<RGBColorF>{};

static auto vertex_p_vec_hasher        = std::hash<VertexArray<Vertex_P>>{};
static auto vertex_p_hasher            = std::hash<Vertex_P>{};
static auto vertex_p_c_vec_hasher      = std::hash<VertexArray<Vertex_P_C>>{};
static auto vertex_p_c_hasher          = std::hash<Vertex_P_C>{};
static auto vertex_p_c_uv_vec_hasher   = std::hash<VertexArray<Vertex_P_C_UV>>{};
static auto vertex_p_c_uv_hasher       = std::hash<Vertex_P_C_UV>{};
static auto vertex_p_c_n_vec_hasher    = std::hash<VertexArray<Vertex_P_C_N>>{};
static auto vertex_p_c_n_hasher        = std::hash<Vertex_P_C_N>{};
static auto index_vec_hasher           = std::hash<IndexArray>{};
static auto large_index_vec_hasher     = std::hash<LargeIndexArray>{};
static auto hardware_buffer_ptr_hasher = std::hash<const HardwareBuffer *>{};
static auto texture_ptr_hasher         = std::hash<const Texture *>{};
static auto stage_hasher               = std::hash<Shader::Stage>{};
static auto bindings_hasher            = std::hash<std::vector<Binding>>{};

static auto bindings_state_hasher         = std::hash<BindingState>{};
static auto program_ptr_hasher            = std::hash<Program *>{};
static auto pipeline_hasher               = std::hash<PipelineState>{};
static auto uniform_buffer_binding_hasher = std::hash<UniformBufferBinding>{};
static auto texture_binding_hasher        = std::hash<TextureBinding>{};
static auto binding_state_hasher          = std::hash<BindingState>{};

namespace std {
    std::size_t hash<Vertex_P>::operator()(const Vertex_P &obj) const {
        auto hash = vec3_hasher(obj.position);

        return hash;
    }

    bool equal_to<Vertex_P>::operator()(const Vertex_P &first, const Vertex_P &second) const
      noexcept {
        return vertex_p_hasher(first) == vertex_p_hasher(second);
    }

    std::size_t hash<Vertex_P_C>::operator()(const Vertex_P_C &obj) const {
        auto hash = vec3_hasher(obj.position);
        hash_combine(hash, vec4_hasher(obj.color));

        return hash;
    }

    bool equal_to<Vertex_P_C>::operator()(const Vertex_P_C &first, const Vertex_P_C &second) const
      noexcept {
        return vertex_p_c_hasher(first) == vertex_p_c_hasher(second);
    }

    std::size_t hash<Vertex_P_C_N>::operator()(const Vertex_P_C_N &obj) const {
        auto hash = vec3_hasher(obj.position);
        hash_combine(hash, vec4_hasher(obj.color));
        hash_combine(hash, vec3_hasher(obj.normal));

        return hash;
    }

    bool equal_to<Vertex_P_C_N>::
    operator()(const Vertex_P_C_N &first, const Vertex_P_C_N &second) const noexcept {
        return vertex_p_c_n_hasher(first) == vertex_p_c_n_hasher(second);
    }

    std::size_t hash<Vertex_P_C_UV>::operator()(const Vertex_P_C_UV &obj) const {
        auto hash = vec3_hasher(obj.position);
        hash_combine(hash, vec4_hasher(obj.color));
        hash_combine(hash, vec2_hasher(obj.uv));

        return hash;
    }

    bool equal_to<Vertex_P_C_UV>::
    operator()(const Vertex_P_C_UV &first, const Vertex_P_C_UV &second) const noexcept {
        return vertex_p_c_uv_hasher(first) == vertex_p_c_uv_hasher(second);
    }

    std::size_t hash<storm::engine::UniformBufferBinding>::
    operator()(const storm::engine::UniformBufferBinding &obj) const {
        auto hash = size_t_hasher(obj.size);
        hash_combine(hash, hardware_buffer_ptr_hasher(obj.buffer));
        hash_combine(hash, ptrdiff_t_hasher(obj.offset));
        hash_combine(hash, stage_hasher(obj.stages));
        hash_combine(hash, uint32_t_hasher(obj.binding));

        return hash;
    }

    bool equal_to<UniformBufferBinding>::
    operator()(const UniformBufferBinding &first, const UniformBufferBinding &second) const
      noexcept {
        return uniform_buffer_binding_hasher(first) == uniform_buffer_binding_hasher(second);
    }

    std::size_t hash<TextureBinding>::operator()(const TextureBinding &obj) const {
        auto hash = uint32_t_hasher(obj.binding);
        hash_combine(hash, texture_ptr_hasher(obj.texture));

        return hash;
    }

    bool equal_to<TextureBinding>::
    operator()(const TextureBinding &first, const TextureBinding &second) const noexcept {
        return texture_binding_hasher(first) == texture_binding_hasher(second);
    }

    std::size_t hash<BindingState>::operator()(const BindingState &obj) const {
        auto hash = bindings_hasher(obj.bindings);

        return hash;
    }

    bool equal_to<BindingState>::
    operator()(const BindingState &first, const BindingState &second) const noexcept {
        return binding_state_hasher(first) == binding_state_hasher(second);
    }
} // namespace std
