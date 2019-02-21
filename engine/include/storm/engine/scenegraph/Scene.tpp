// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "Scene.hpp"

namespace storm::engine {
    template<typename T, typename... Args>
    T &Scene::createNode(Args &&... args) {
        auto &node_ref = *m_nodes.emplace_back(new T{*this, std::forward<Args>(args)...});

        return static_cast<T &>(node_ref);
    }
} // namespace storm::engine
