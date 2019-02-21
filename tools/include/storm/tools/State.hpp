#pragma once
/* @author Arthapz
 * @version 0.1
 **/

#include <memory>
#include <stack>
#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Platform.hpp>

namespace storm::tools {
    class StateManager;
    /**
     * @class State
     * @ingroup tools
     * @brief The Abstract State class
     *
     * The class is the base of all State
     */
    class STORM_PUBLIC State : public core::NonCopyable,
                               public std::enable_shared_from_this<State> {
    public:
        SUR_Object(State)
          /**
           * @brief Constructor
           *
           * Constructor of State
           * @param parent : the state manager parent to this state
           * (StateManager)
           */
          explicit State(StateManager &owner);

        /**
         * @brief Move Constructor
         *
         * Move Constructor of State
         *
         * @param state : the state which be moved (State rv)
         */
        State(State &&state) = default;

        /**
         * @brief Move assignement
         *
         * Move assignement of State
         *
         * @param state : the state which be moved (State rv)
         * @return a reference to current instance
         */
        State &operator=(State &&state) = default;

        /**
         * @brief Destructor
         *
         * Destructor of State
         */
        virtual ~State() noexcept = default;

        /**
         * @brief pause the state
         *
         * Method which pause the state
         */
        virtual void pause() = 0;

        /**
         * @brief resume the state
         *
         * Method which resume the state
         */
        virtual void resume() = 0;

        /**
         * @brief update the state
         *
         * Method which update the state data
         *
         * @param delta : the time since last update in microsecond(64 bit
         * unsigned integer)
         */
        virtual void update(std::uint64_t delta = 0U) = 0;

        /**
         * @brief render the state
         *
         * Method which render the state data
         *
         */
        virtual void render() = 0;

    protected:
        StateManager &m_owner;
    };
} // namespace storm::tools
