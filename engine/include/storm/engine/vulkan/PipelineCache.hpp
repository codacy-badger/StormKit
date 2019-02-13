// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "UniqueHandle.hpp"

#include <storm/core/Hash.hpp>
#include <storm/core/Numerics.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <unordered_map>

namespace storm::engine {
	class Program;
	struct PipelineCacheKey {
		const Program *            program;
		const RenderPass *         render_pass;
		const RenderPass::SubPass *subpass;

		bool operator==(const PipelineCacheKey &other) const noexcept;
	};
}

namespace std {
	template <>
	struct hash<storm::engine::PipelineCacheKey> {
		std::size_t operator()(
		    const storm::engine::PipelineCacheKey &key) const {
			auto hash = std::hash<const storm::engine::RenderPass *> {}(
			    key.render_pass);
			storm::core::hash_combine(
			    hash, std::hash<const storm::engine::RenderPass::SubPass *> {}(
			              key.subpass));
			storm::core::hash_combine(hash,
			    std::hash<const storm::engine::Program *> {}(key.program));

			return hash;
		}
	};
}

namespace storm::engine {
	class PipelineCache : public storm::core::NonCopyable {
	public:
		explicit PipelineCache();
		~PipelineCache();

		PipelineCache(PipelineCache &&);
		PipelineCache &operator=(PipelineCache &&);

		void add(
		    const PipelineCacheKey &key, UniqueHandle<vk::Pipeline> &&pipeline);
		bool                has(const PipelineCacheKey &key) const;
		const vk::Pipeline &acquire(const PipelineCacheKey &key) const;

	private:
		std::unordered_map<PipelineCacheKey, UniqueHandle<vk::Pipeline>>
		    m_pipeline_cache;
	};
}
