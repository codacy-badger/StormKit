// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/RenderTaskBase.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase::RenderTaskBase(std::string name)
	: m_name{std::move(name)}, m_cull_imune{false}, m_id{0u}, m_ref_count{0u} {}

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase::~RenderTaskBase() = default;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase::RenderTaskBase(RenderTaskBase &&) = default;
/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase &RenderTaskBase::operator=(RenderTaskBase &&) = default;
