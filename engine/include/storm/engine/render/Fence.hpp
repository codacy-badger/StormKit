// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>

namespace storm::engine {
    class FenceImpl;
    class Fence {
    public:
        Unique_Object(Fence) explicit Fence(const Device &device);
        ~Fence();

        Fence(Fence &&);
        Fence &operator=(Fence &&);

        void wait(std::uint64_t timeout = std::numeric_limits<std::uint64_t>::max());
        void reset();

        IMPLEMENTATION(FenceImpl)
    private:
        core::Pimpl<FenceImpl> m_impl;
    };
} // namespace storm::engine
