#pragma once
/**
 * @file NonInstanciable.hpp
 * @brief NonInstanciable Tools
 * @author Arthapz
 * @version 0.1
 **/

#include <storm/core/NonCopyable.hpp>
#include <storm/core/NonMovable.hpp>

namespace storm::core {
    /**
     * @class NonInstanciable
     * @ingroup Core
     * @brief NonInstanciable class helpers
     *
     * The class allow to a class to be non instanciable
     */
    class NonInstanciable : public NonMovable, public NonCopyable {
    public:
        /**
         * @brief Constructor
         *
         * Default Constructor of NonInstanciable
         */
        NonInstanciable() = delete;
    };
} // namespace storm::core
