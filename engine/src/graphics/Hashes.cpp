// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/Hashes.hpp>
#include <storm/engine/graphics/Material.hpp>
#include <storm/engine/graphics/DirectionalLight.hpp>
#include <storm/engine/graphics/Drawable.hpp>
#include <storm/engine/graphics/Geometry.hpp>
#include <storm/engine/graphics/Transform.hpp>
#include <storm/engine/graphics/RendererCommandBufferCacheKey.hpp>
#include <storm/engine/render/Hashes.hpp>

#include <storm/core/TypeTraits.hpp>

#include <storm/core/Numerics.hpp>

using namespace storm;
using namespace storm::core;
using namespace storm::engine;

static auto uint32_t_hasher = std::hash<std::uint32_t>{};
static auto ptrdiff_t_hasher = std::hash<std::ptrdiff_t>{};
static auto size_t_hasher = std::hash<std::size_t>{};
static auto float_hasher = std::hash<float>{};
static auto color_hasher = std::hash<RGBColorF>{};
static auto geometry_hasher = std::hash<Geometry>{};
static auto program_ptr_hasher = std::hash<Program*>{};
static auto pipeline_hasher = std::hash<PipelineState>{};
static auto vec2_hasher = std::hash<vec2>{};
static auto vec3_hasher = std::hash<vec3>{};
static auto vec4_hasher = std::hash<vec4>{};
static auto mat4_hasher = std::hash<mat4>{};
static auto quat_hasher = std::hash<quat>{};
static auto vertex_p_vec_hasher = std::hash<VertexArray<Vertex_P>>{};
static auto vertex_p_hasher = std::hash<Vertex_P>{};
static auto vertex_p_c_vec_hasher = std::hash<VertexArray<Vertex_P_C>>{};
static auto vertex_p_c_hasher = std::hash<Vertex_P_C>{};
static auto vertex_p_c_uv_vec_hasher = std::hash<VertexArray<Vertex_P_C_UV>>{};
static auto vertex_p_c_uv_hasher = std::hash<Vertex_P_C_UV>{};
static auto vertex_p_c_n_vec_hasher = std::hash<VertexArray<Vertex_P_C_N>>{};
static auto vertex_p_c_n_hasher = std::hash<Vertex_P_C_N>{};
static auto index_vec_hasher = std::hash<IndexArray>{};
static auto large_index_vec_hasher = std::hash<LargeIndexArray>{};
static auto uniform_buffer_ptr_hasher = std::hash<UniformBuffer*>{};
static auto texture_ptr_hasher = std::hash<Texture*>{};
static auto stage_hasher = std::hash<Shader::Stage>{};
static auto bindings_hasher = std::hash<std::vector<Binding>>{};
static auto bindings_state_hasher = std::hash<BindingState>{};
static auto material_hasher = std::hash<Material>{};
static auto drawable_hasher = std::hash<Drawable>{};
static auto directional_light_hasher = std::hash<DirectionalLight>{};
static auto transform_hasher = std::hash<Transform>{};
static auto renderer_command_buffer_key_hasher = std::hash<RendererCommandBufferCacheKey>{};
static auto uniform_buffer_binding_hasher = std::hash<UniformBufferBinding>{};
static auto texture_binding_hasher = std::hash<TextureBinding>{};
static auto binding_state_hasher = std::hash<BindingState>{};

namespace std {
	std::size_t hash<Material>::operator()(const Material& obj) const {
		auto hash = color_hasher(obj.m_ambiant);
		hash_combine(hash, color_hasher(obj.m_diffuse));
		hash_combine(hash, color_hasher(obj.m_specular));
		hash_combine(hash, color_hasher(obj.m_emissive));
		hash_combine(hash, float_hasher(obj.m_shinyness));
		
		return hash;
	}
	
	bool equal_to<Material>::operator()(const Material &first, const Material &second) const noexcept { 
		return material_hasher(first) == material_hasher(second);
	}
	
	std::size_t hash<Drawable>::operator()(const Drawable& obj) const {
		auto hash = geometry_hasher(obj.geometry);
		hash_combine(hash, program_ptr_hasher(obj.program));
		hash_combine(hash, pipeline_hasher(obj.pipeline_state));
		hash_combine(hash, bindings_state_hasher(obj.binding_state));
		
		return hash;
	}
	
	bool equal_to<Drawable>::operator()(const Drawable &first, const Drawable &second) const noexcept { 
		return drawable_hasher(first) == drawable_hasher(second);
	}
	
	std::size_t hash<DirectionalLight>::operator()(const DirectionalLight& obj) const {
		auto hash = color_hasher(obj.m_ambiant);
		hash_combine(hash, color_hasher(obj.m_diffuse));
		hash_combine(hash, vec3_hasher(obj.m_position));
		hash_combine(hash, color_hasher(obj.m_specular));
		
		return hash;
	}
	
	bool equal_to<DirectionalLight>::operator()(const DirectionalLight &first, const DirectionalLight &second) const noexcept { 
		return directional_light_hasher(first) == directional_light_hasher(second);
	}
	
	std::size_t hash<Geometry>::operator()(const Geometry& obj) const {
		auto hash = std::size_t{0};
		std::visit(
			overload {
				[&hash](const VertexArray<Vertex_P> &vertices)
				{ hash = vertex_p_vec_hasher(vertices); },
				[&hash](const VertexArray<Vertex_P_C> &vertices)
				{ hash = vertex_p_c_vec_hasher(vertices); },
				[&hash](const VertexArray<Vertex_P_C_N> &vertices)
				{ hash = vertex_p_c_n_vec_hasher(vertices); },
				[&hash](const VertexArray<Vertex_P_C_UV> &vertices)
				{ hash = vertex_p_c_uv_vec_hasher(vertices); }
			},
			obj.vertices
		);
		
		std::visit(
			overload {
				[&hash](const IndexArray &index)
				{ hash_combine(hash,index_vec_hasher(index)); },
				[&hash](const LargeIndexArray &index)
				{ hash_combine(hash,large_index_vec_hasher(index)); }
			},
			obj.indices
		);
		
		return hash;
	}
	
	bool equal_to<Geometry>::operator()(const Geometry &first, const Geometry &second) const noexcept { 
		return geometry_hasher(first) == geometry_hasher(second);
	}
	
	std::size_t hash<Transform>::operator()(const Transform& obj) const {
		auto hash = float_hasher(obj.m_yaw);
		hash_combine(hash, float_hasher(obj.m_roll));
		hash_combine(hash, float_hasher(obj.m_pitch));
		hash_combine(hash, vec3_hasher(obj.m_scale));
		hash_combine(hash, vec3_hasher(obj.m_origin));
		hash_combine(hash, vec3_hasher(obj.m_position));
		
		return hash;
	}
	
	bool equal_to<Transform>::operator()(const Transform &first, const Transform &second) const noexcept { 
		return transform_hasher(first) == transform_hasher(second);
	}
	
	std::size_t hash<Vertex_P>::operator()(const Vertex_P& obj) const {
		auto hash = vec3_hasher(obj.position);
		
		return hash;
	}
	
	bool equal_to<Vertex_P>::operator()(const Vertex_P &first, const Vertex_P &second) const noexcept { 
		return vertex_p_hasher(first) == vertex_p_hasher(second);
	}
	
	std::size_t hash<Vertex_P_C>::operator()(const Vertex_P_C& obj) const {
		auto hash = vec3_hasher(obj.position);
		hash_combine(hash, vec4_hasher(obj.color));
				
		return hash;
	}
	
	bool equal_to<Vertex_P_C>::operator()(const Vertex_P_C &first, const Vertex_P_C &second) const noexcept { 
		return vertex_p_c_hasher(first) == vertex_p_c_hasher(second);
	}

	std::size_t hash<Vertex_P_C_N>::operator()(const Vertex_P_C_N& obj) const {
		auto hash = vec3_hasher(obj.position);
		hash_combine(hash, vec4_hasher(obj.color));
		hash_combine(hash, vec3_hasher(obj.normal));
		
		return hash;
	}
	
	bool equal_to<Vertex_P_C_N>::operator()(const Vertex_P_C_N &first, const Vertex_P_C_N &second) const noexcept { 
		return vertex_p_c_n_hasher(first) == vertex_p_c_n_hasher(second);
	}
	
	std::size_t hash<Vertex_P_C_UV>::operator()(const Vertex_P_C_UV& obj) const {
		auto hash = vec3_hasher(obj.position);
		hash_combine(hash, vec4_hasher(obj.color));
		hash_combine(hash, vec2_hasher(obj.uv));
		
		return hash;
	}
	
	bool equal_to<Vertex_P_C_UV>::operator()(const Vertex_P_C_UV &first, const Vertex_P_C_UV &second) const noexcept { 
		return vertex_p_c_uv_hasher(first) == vertex_p_c_uv_hasher(second);
	}
	
	std::size_t hash<RendererCommandBufferCacheKey>::operator()(const RendererCommandBufferCacheKey& obj) const {
		auto hash = size_t_hasher(obj.vertex_count);
		hash_combine(hash, size_t_hasher(obj.instance_count));
		hash_combine(hash, size_t_hasher(obj.index_count));
		hash_combine(hash, pipeline_hasher(obj.state));
		
		return hash;
	}
	
	bool equal_to<RendererCommandBufferCacheKey>::operator()(const RendererCommandBufferCacheKey &first, const RendererCommandBufferCacheKey &second) const noexcept { 
		return renderer_command_buffer_key_hasher(first) == renderer_command_buffer_key_hasher(second);
	}
	
	std::size_t hash<storm::engine::UniformBufferBinding>::operator()(const storm::engine::UniformBufferBinding& obj) const {
		auto hash = size_t_hasher(obj.size);
		hash_combine(hash, uniform_buffer_ptr_hasher(obj.buffer));
		hash_combine(hash, ptrdiff_t_hasher(obj.offset));
		hash_combine(hash, stage_hasher(obj.stages));
		hash_combine(hash, uint32_t_hasher(obj.binding));
		
		return hash;
	}
	
	bool equal_to<UniformBufferBinding>::operator()(const UniformBufferBinding &first, const UniformBufferBinding &second) const noexcept { 
		return uniform_buffer_binding_hasher(first) == uniform_buffer_binding_hasher(second);
	}
	
	std::size_t hash<TextureBinding>::operator()(const TextureBinding& obj) const {
		auto hash = uint32_t_hasher(obj.binding);
		hash_combine(hash, texture_ptr_hasher(obj.texture));
		
		return hash;
	}
	
	bool equal_to<TextureBinding>::operator()(const TextureBinding &first, const TextureBinding &second) const noexcept { 
		return texture_binding_hasher(first) == texture_binding_hasher(second);
	}
	
	std::size_t hash<storm::engine::BindingState>::operator()(const storm::engine::BindingState& obj) const {
		auto hash = bindings_hasher(obj.bindings);
		
		return hash;
	}
	
	bool equal_to<BindingState>::operator()(const BindingState &first, const BindingState &second) const noexcept { 
		return binding_state_hasher(first) == binding_state_hasher(second);
	}
}
