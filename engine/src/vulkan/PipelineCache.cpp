// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/PipelineCache.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
PipelineCache::PipelineCache() {
	
}

/////////////////////////////////////
/////////////////////////////////////
PipelineCache::~PipelineCache() = default;

/////////////////////////////////////
/////////////////////////////////////
PipelineCache::PipelineCache(PipelineCache &&) = default;

/////////////////////////////////////
/////////////////////////////////////
PipelineCache &PipelineCache::operator=(PipelineCache &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void PipelineCache::add(const PipelineCacheKey &key, UniqueHandle<vk::Pipeline> &&pipeline) {
	m_pipeline_cache.emplace(key, std::move(pipeline));
}

/////////////////////////////////////
/////////////////////////////////////
bool PipelineCache::has(const PipelineCacheKey &key) const {
	auto it = m_pipeline_cache.find(key);
	
	return it != std::end(m_pipeline_cache);
}

/////////////////////////////////////
/////////////////////////////////////
const vk::Pipeline &PipelineCache::acquire(const PipelineCacheKey &key) const {
	return m_pipeline_cache.at(key).get();
}
