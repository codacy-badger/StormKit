// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "Subject.hpp"

namespace storm::tools {
    template<typename Event, typename CustomData>
    inline void Subject<Event, CustomData>::resetObserver() {
        m_observer = nullptr;
    }

    template<typename Event, typename CustomData>
    inline typename Subject<Event, CustomData>::ObserverCRef
    Subject<Event, CustomData>::observer() const noexcept {
        return *m_observer;
    }

    template<typename Event, typename CustomData>
    inline typename Subject<Event, CustomData>::ObserverRef
    Subject<Event, CustomData>::observer() noexcept {
        return *m_observer;
    }

    template<typename Event, typename CustomData>
    inline typename Subject<Event, CustomData>::ObserverPtr
    Subject<Event, CustomData>::observerPtr() noexcept {
        return m_observer;
    }
} // namespace storm::tools
