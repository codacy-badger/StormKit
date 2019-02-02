#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <queue>

#include <storm/core/Memory.hpp>
#include <storm/tools/Observer.hpp>

namespace storm::tools {
	template <typename Event, typename CustomData>
	class Subject {
		public:
			using PtrType = std::shared_ptr<Observer<Event, CustomData>>;

			explicit Subject();

			Subject(const Subject &subject)  = default;
			Subject(Subject &&subject)       = default;

			Subject &operator=(const Subject &subject) = default;
			Subject &operator=(Subject &&subject)      = default;

			void registerObserver(PtrType &&observer);
			inline void resetObserver() { m_observer.reset(); }
			void notify(Event &&event, CustomData &&args = nullptr);
			void notifyDefferedEvents();

			inline const PtrType &observer() const noexcept { return m_observer; }
			inline       PtrType  observer() noexcept { return m_observer; }

			void defferEvent(Event &&event, CustomData &&args = nullptr);
		private:
			PtrType m_observer;

			std::queue<std::pair<Event, CustomData>> m_deffered_events;
	};
}

#include <storm/tools/Subject.tpp>
