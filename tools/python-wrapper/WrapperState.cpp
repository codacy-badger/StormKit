#include "WrapperTree.hpp"

#include <storm/tools/StateManager.hpp>

namespace py = pybind11;

using namespace storm::tools;

/*
explicit State(StateManager &owner);
virtual void pause() = 0;
virtual void resume() = 0;
virtual void update(std::uint64_t delta) = 0;
virtual void render() = 0;*/

class PyState: public State {
public:
		Shared_Object(PyState)

		using State::State;
		using State::m_owner;

		void pause() override {
			PYBIND11_OVERLOAD_PURE(
				void,
				State,
				pause
			);
		}
		void resume() override {
			PYBIND11_OVERLOAD_PURE(
				void,
				State,
				resume
			);
		}
		void update([[maybe_unused]] std::uint64_t delta = 0U) override {
			PYBIND11_OVERLOAD_PURE(
				void,
				State,
				update,
				delta
			);
		}
		void render() override {
			PYBIND11_OVERLOAD_PURE(
				void,
				State,
				render
			);
		}
};

void initStateBinding(py::module &module) noexcept {
	py::class_<State, PyState, std::shared_ptr<State>>(module, "State")
		.def(py::init<StateManager&>())
		.def_property_readonly("owner", [](PyState &state) -> StateManager& { return state.m_owner;} )
		.def("pause",  &State::pause)
		.def("resume", &State::resume)
		.def("update", &State::update)
		.def("render", &State::render);

	py::class_<StateManager>(module, "StateManager")
		.def(py::init<>())
		.def("requestPush",     [](StateManager &manager, const State::Ptr &state) { state->update(); manager.requestPush(state); })
		.def("requestSet",      [](StateManager &manager, const State::Ptr &state) { manager.requestSet(state); })
		.def("executeRequests", &StateManager::executeRequests)
		.def("update",          &StateManager::update, py::arg_v("delta", 0U))
		.def("render",          &StateManager::render)
		//.def("size",            &StateManager::size)
		.def("requestClear",    &StateManager::requestClear)
		.def("top",             [](StateManager &manager) -> PyState::Ptr { return std::dynamic_pointer_cast<PyState>(manager.top().shared_from_this()); });
}
