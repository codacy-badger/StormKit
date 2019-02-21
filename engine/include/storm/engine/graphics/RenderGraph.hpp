// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/engine/graphics/ForwardDeclarations.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>

namespace storm::engine {
    class RenderGraph : public core::NonCopyable {
    public:
        explicit RenderGraph();
        ~RenderGraph();

        RenderGraph(RenderGraph &&);
        RenderGraph &operator=(RenderGraph &&);

    private:
    };
} // namespace storm::engine
