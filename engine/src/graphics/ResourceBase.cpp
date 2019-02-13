// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/ResourceBase.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ResourceBase(
    std::string name, RenderTaskBaseOptionalRef &&creator)
    : m_name {std::move(name)}, m_creator {std::move(creator)}, m_id {0u},
      m_ref_count {0u} {}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::~ResourceBase() = default;

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ResourceBase(ResourceBase &&) = default;
/////////////////////////////////////
/////////////////////////////////////
ResourceBase &ResourceBase::operator=(ResourceBase &&) = default;
