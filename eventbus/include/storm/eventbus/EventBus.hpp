#pragma once

#include <functional>
#include <queue>
#include <storm/core/NonCopyable.hpp>
#include <unordered_map>

namespace storm::eventbus {
    template<typename EventType, typename EventPayload>
    class EventBus final : storm::core::NonCopyable {
    public:
        using EventHandler  = std::function<void(EventPayload)>;
        explicit EventBus() = default;

        EventBus(EventBus &&) = default;
        EventBus &operator=(EventBus &&) = default;

        ~EventBus() = default;

        void emit(EventType event, EventPayload payload);

        void subscribe(EventType event, EventHandler handler);
        void unsubscribe(EventHandler handler);

        void clearAllSubscribers(EventType event);
        void clearAllSubscribers();

        void notify();

    private:
        std::unordered_map<EventType, std::vector<EventHandler>> m_handlers;
        std::queue<std::pair<EventType, EventPayload>> m_event_queue;
    };
} // namespace storm::eventbus

#include "EventBus.tpp"
