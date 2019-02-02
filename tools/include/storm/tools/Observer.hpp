#pragma once

#include <string>
#include <functional>

#include <storm/core/Memory.hpp>

namespace storm::tools {
	template <typename Event, typename CustomData>
	class Subject;

	class UnhandledEventException : public std::runtime_error {
		public:
			UnhandledEventException(const std::string &event_value) : std::runtime_error("Unhandled Event ! value = " + event_value) {}
			~UnhandledEventException() override;
	};
	template <typename Event, typename CustomData>
	class Observer {
		protected:
			using SharedObjectObserverHelper = Observer<Event, CustomData>;
		public:
			Shared_Object_Wm(SharedObjectObserverHelper)
			Unique_Object_Wm(SharedObjectObserverHelper)

			virtual ~Observer() = default;

			virtual void onNotified(Event, CustomData &&) = 0;
	};
}
