// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

namespace storm::engine {
	class DirectionalLight;
	class Drawable;
	class DrawableCache;
	class Geometry;
	class Material;
	template <auto Method>
	class Renderer;
	class Transform;
	class RenderTaskBase;
	class RenderGraph;
	class ResourcePool;
	class ResourceBase;
	class RenderTaskBuilder;
	template <typename ResourceDescription_, typename ResourceType_>
	class Resource;
	template <typename RenderTaskData>
	class RenderTask;
	
	struct Vertex_P;
	struct Vertex_P_C;
	struct Vertex_P_C_N;
	struct Vertex_P_C_UV;
	struct Vertex_P_UV;
	struct Vertex_P_UV_N;
	struct Vertex_P_UV_N_T_BT;
	struct RendererCommandBufferCacheKey;
}
