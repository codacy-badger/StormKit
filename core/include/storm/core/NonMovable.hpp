#pragma once
/**
 * @file NonCopyable.hpp
 * @brief NonCopyable Tools
 * @author Arthapz
 * @version 0.1
 **/

#include <storm/core/Platform.hpp>

namespace storm::core {
    /**
     * @class NonMovable
     * @ingroup Core
     * @brief NonMovable class helpers
     *
     * The class allow to a class to be non movable
     */

    class STORM_PUBLIC NonMovable {
    public:
        /**
         * @brief Constructor
         *
         * Default Constructor of NonMovable
         */
        constexpr NonMovable() noexcept = default;

        /**
         * @brief Destructor
         *
         * Destructor of NonMovable
         */
        virtual ~NonMovable() noexcept = default;

        /**
         * @brief Copy Constructor
         *
         * Copy Constructor of NonMovable
         */
        NonMovable(NonMovable &) noexcept = default;

        /**
         * @brief Copy Assignement
         *
         * Copy Assignement of NonMovable
         */
        NonMovable &operator=(const NonMovable &) noexcept = default;

        /**
         * @brief Move Constructor
         *
         * Move Constructor of NonMovable
         */
        NonMovable(NonMovable &&) = delete;

        /**
         * @brief Move Assignement
         *
         * Move Assignement of NonMovable
         */
        NonMovable &operator=(NonMovable &&) = delete;
    };
} // namespace storm::core
