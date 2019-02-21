// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "SceneNode.hpp"

#include <storm/core/Assert.hpp>
#include <storm/core/Strings.hpp>

namespace storm::engine {
    template<typename T>
    T &SceneNode::childAs(ID id) {
        auto it =
          std::find_if(std::begin(m_children), std::end(m_children), [&](const auto &child_) {
              return id == child_.get().id();
          });

        ASSERT(it != std::end(m_children), core::format("Failed to find node, with id: %{1}", id));

        return static_cast<T &>(it->get());
    }
} // namespace storm::engine
