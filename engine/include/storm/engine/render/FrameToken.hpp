// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <cstddef>
#include <cstdint>
#include <storm/engine/render/ForwardDeclarations.hpp>

namespace storm::engine {
struct FrameToken {
  const std::uint64_t id;
  const std::uint32_t image_index;
  const std::size_t image_count;

  Semaphore &image_available;
  Semaphore &render_finished;
  Fence &fence;
};
} // namespace storm::engine
