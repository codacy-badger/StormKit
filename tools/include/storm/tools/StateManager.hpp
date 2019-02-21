#pragma once
/**
 * @file StateManager.hpp
 * @brief the state manager
 * @author Arthapz
 * @version 0.1
 **/

#include <functional>
#include <memory>
#include <queue>
#include <stack>
#include <storm/core/NonCopyable.hpp>
#include <storm/tools/State.hpp>

namespace storm::tools {
    /**
     * @class StateManager
     * @ingroup tools
     * @brief The State Manager
     *
     * The class handle states like a stack
     */
    class STORM_PUBLIC StateManager final : public core::NonCopyable {
    public:
        /**
         * @brief Default Constructor
         *
         * Default Constructor of State
         */
        explicit StateManager() = default;

        /**
         * @brief Move Constructor
         *
         * Move Constructor of StateManager
         *
         * @param manager : the state manager which be moved (StateManager rv)
         */
        StateManager(StateManager &&manager) = default;

        /**
         * @brief Move assignement
         *
         * Move assignement of StateManager
         *
         * @param manager : the state manager which be moved (StateManager rv)
         * @return a reference to current instance
         **/
        StateManager &operator=(StateManager &&manager) = default;

        /**
         * @brief Destructor
         *
         * Destructor of StateManager
         */
        ~StateManager() = default;

        /**
         * @brief push the stack
         *
         * Method which create the state object and place it in Queue: the next
         * executeRequests call will push it
         *
         * @param args : the state args (variadic)
         */
        template<class T, typename... Args>
        inline void requestPush(Args &&... args) {
            static_assert(
              std::is_base_of<State, T>::value, "T must inherit from storm::tools::State");

            requestPush(T::makeUnique(*this, std::forward<Args>(args)...));
        }

        /**
         * @brief push the stack
         *
         * Method which make a state and place it in Queue: the next
         * executeRequests call will push it
         *
         * @param state : the state
         */
        void requestPush(State::Ptr &&state);

        /**
         * @brief set the stack
         *
         * Method wich create the state object it in Queue: the next
         * executeRequests call will set it
         *
         * @param args : the state args (variadic)
         */
        template<class T, typename... Args>
        inline void requestSet(Args &&... args) {
            static_assert(
              std::is_base_of<State, T>::value, "T must inherit from storm::tools::State");

            requestSet(T::makeUnique(*this, std::forward<Args>(args)...));
        }

        /**
         * @brief set the stack
         *
         * Method wich create the state and place it in Queue: the next
         * executeRequests call will set it
         *
         * @param state : the state
         */
        void requestSet(State::Ptr &&state);

        /**
         * @brief pop the stack
         *
         * Method which make a state and place it in queue: the next
         * executeRequests call will pop it
         */
        void requestPop();

        /**
         * @brief execute requests
         *
         * Method which execute push, pop, set requested in the queue
         */
        void executeRequests();

        /**
         * @brief update the top state
         *
         * Method which update the top state data
         *
         * @param delta : the time since last update (float) default(0.f)
         */
        void update(std::uint64_t delta = 0U);

        /**
         * @brief render the top state
         *
         * Method which render the top state data
         *
         */
        void render();

        void requestClear();

        /**
         * @brief get the top state
         *
         * Method which get a reference to the top state
         *
         * @return a reference to te top state
         */
        State &top() noexcept;

        template<class T>
        T &top() {
            static_assert(
              std::is_base_of<State, T>::value, "T must inherit from storm::tools::State");

            return *static_cast<T *>(m_stack.top().get());
        }

        inline std::size_t size() const noexcept { return m_stack.size(); }

    private:
        struct StateManagerAction {
            enum class Type { push, pop, set, clear } type;
            std::unique_ptr<State> state; // Null when pop
        };

        std::queue<StateManagerAction> m_actionQueue;
        std::stack<std::unique_ptr<State>> m_stack;
    };
} // namespace storm::tools
