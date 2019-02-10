#include <storm/tools/Subject.hpp>

#include <utility>

namespace storm::tools {
	////////////////////////////////////////
	////////////////////////////////////////
	template <typename Event, typename CustomData>
	Subject<Event, CustomData>::Subject() : m_observer(nullptr) {

	};

	////////////////////////////////////////
	////////////////////////////////////////
	template <typename Event, typename CustomData>
	void Subject<Event, CustomData>::registerObserver(ObserverPtr observer) {
		m_observer = observer;

		notifyDefferedEvents();
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <typename Event, typename CustomData>
	void Subject<Event, CustomData>::notify(Event &&event, CustomData&& args) {
		if(m_observer)
			m_observer->onNotified(event, std::move(args));
		else
			defferEvent(std::move(event), std::move(args));
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template<typename Event, typename CustomData>
	void Subject<Event, CustomData>::notifyDefferedEvents() {
		while(!m_deffered_events.empty()) {
			auto pair = std::move(m_deffered_events.front());

			//if(pair.first != nullptr)
				m_observer->onNotified(pair.first, std::move(pair.second));

			m_deffered_events.pop();
		}
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template<typename Event, typename CustomData>
	void Subject<Event, CustomData>::defferEvent(Event &&event, CustomData &&args) {
		m_deffered_events.emplace(std::move(event), std::move(args));
	}
}

