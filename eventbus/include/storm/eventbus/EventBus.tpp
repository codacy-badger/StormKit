#include <storm/eventbus/EventBus.hpp>

namespace storm::eventbus {
    template<typename EventType, typename EventPayload>
    void EventBus<EventType, EventPayload>::emit(EventType event, EventPayload payload) {
        m_event_queue.emplace(event, std::move(payload));
    }

    template<typename EventType, typename EventPayload>
    void EventBus<EventType, EventPayload>::subscribe(EventType event, EventHandler handler) {
        m_handlers[ event ].emplace_back(std::move(handler));
    }

    template<typename EventType, typename EventPayload>
    void EventBus<EventType, EventPayload>::unsubscribe(EventHandler handler) {
        std::for_each(std::begin(m_handlers), std::end(m_handlers), [handler](auto &handlers) {
            std::remove(std::begin(handlers), std::end(handlers), handler);
        });
    }

    template<typename EventType, typename EventPayload>
    void EventBus<EventType, EventPayload>::clearAllSubscribers(EventType event) {
        m_handlers[ event ].clear();
    }

    template<typename EventType, typename EventPayload>
    void EventBus<EventType, EventPayload>::clearAllSubscribers() {
        m_handlers.clear();
    }

    template<typename EventType, typename EventPayload>
    void EventBus<EventType, EventPayload>::notify() {
        while(!m_event_queue.empty()) {
            auto event = m_event_queue.front();
            m_event_queue.pop();

            for(auto &handler : m_handlers[ event.first ]) { handler(event.second); }
        }
    }
} // namespace storm::eventbus
