#pragma once
#include <functional>
#include <queue>
#include <storm/core/Memory.hpp>
#include <storm/tools/Observer.hpp>
#include <unordered_map>
#include <vector>

namespace storm::tools {
	template <typename Event, typename CustomData>
	class Subject {
	public:
		using ObserverType = Observer<Event, CustomData>;
		using ObserverPtr  = typename ObserverType::RawPtr;
		using ObserverRef  = typename ObserverType::Ref;
		using ObserverCRef = typename ObserverType::Ptr;

		explicit Subject();

		Subject(const Subject &subject) = default;
		Subject(Subject &&subject)      = default;

		Subject &operator=(const Subject &subject) = default;
		Subject &operator=(Subject &&subject) = default;

		void registerObserver(ObserverPtr observer);

		void notify(Event &&event, CustomData &&args = nullptr);
		void notifyDefferedEvents();

		inline void         resetObserver();
		inline ObserverCRef observer() const noexcept;
		inline ObserverRef  observer() noexcept;
		inline ObserverPtr  observerPtr() noexcept;

		void defferEvent(Event &&event, CustomData &&args = nullptr);

	private:
		ObserverPtr m_observer;

		std::queue<std::pair<Event, CustomData>> m_deffered_events;
	};
}

#include "Subject.inl"
#include "Subject.tpp"
