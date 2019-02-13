#pragma once

#include <atomic>
#include <chrono>
#include <ciso646>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <set>
#include <storm/core/NonCopyable.hpp>
#include <thread>
#include <unordered_map>

namespace storm::core {
	inline std::uint64_t getNextDeferredTaskId() noexcept {
		static std::uint64_t m_next_id;
		return m_next_id++;
	}

	template <class Clock = std::chrono::high_resolution_clock,
	    class Duration    = std::chrono::milliseconds>
	class Timer : public storm::core::NonCopyable {
		using Timestamp  = std::chrono::time_point<Clock>;
		using Callback   = std::function<void()>;
		using ScopedLock = std::unique_lock<std::mutex>;

	public:
		using DeferredTaskId = std::uint64_t;

		explicit Timer();
		virtual ~Timer();

		Timer(Timer &&timer) = default;
		Timer &operator=(Timer &&timer) = default;

		DeferredTaskId makeTask(
		    Duration when, Duration Period, const Callback &callback);
		DeferredTaskId makeTask(
		    Duration when, Duration Period, Callback &&callback);
		void deleteTask(DeferredTaskId id);

		bool existTask(DeferredTaskId id);

	private:
		struct DeferredTask : public storm::core::NonCopyable {
			template <typename Func>
			explicit DeferredTask(DeferredTaskId id, Timestamp next,
			    Duration duration, Func &&callback) noexcept
			    : id(id), next(next), duration(duration),
			      callback(std::forward<Func>(callback)), running(false) {}

			DeferredTask(DeferredTask &&task) noexcept
			    : id(task.id), next(task.next), duration(task.duration),
			      callback(std::move(task.callback)), running(task.running) {}

			DeferredTask &operator=(DeferredTask &&task) noexcept {
				if (&task != *this) {
					id       = task.id;
					next     = task.next;
					duration = task.duration;
					callback = std::move(task.callback);
					running  = task.running;
				}

				return *this;
			}

			DeferredTaskId id;
			Timestamp      next;
			Duration       duration;
			Callback       callback;
			bool           running;

			inline friend bool operator<(
			    const DeferredTask &a, const DeferredTask &b) {
				return a.next < b.next;
			}

			using Ref = std::reference_wrapper<DeferredTask>;
		};

		DeferredTaskId makeTask(DeferredTask &&task);

		void workerTask();

		bool                    m_stop;
		std::mutex              m_lock;
		std::thread             m_worker;
		std::condition_variable m_wake_up;

		std::unordered_map<DeferredTaskId, DeferredTask> m_tasks;
		std::multiset<typename DeferredTask::Ref, std::less<DeferredTask>>
		    m_task_queue;
	};
}

#include "Timer.tpp"
