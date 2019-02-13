// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "ResourceBase.hpp"

namespace storm::engine {
	inline ResourceBase::ID ResourceBase::id() const noexcept { return m_id; }

	inline bool ResourceBase::transient() const noexcept {
		return m_creator != std::nullopt;
	}

	inline void ResourceBase::setName(std::string name) {
		m_name = std::move(name);
	}

	inline const std::string &ResourceBase::name() const noexcept {
		return m_name;
	}

	inline ResourceBase::ID ResourceBase::typeID() const noexcept {
		return m_type_id;
	}
}
