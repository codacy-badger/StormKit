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
     * @class NonCopyable
     * @ingroup Core
     * @brief NonCopyable class helpers
     *
     * The class allow to a class to be non copyable
     */
    class STORM_PUBLIC NonCopyable {
    public:
        /**
         * @brief Constructor
         *
         * Default Constructor of NonCopyable
         */
        constexpr NonCopyable() noexcept = default;

        /**
         * @brief Destructor
         *
         * Destructor of NonCopyable
         */
        ~NonCopyable() noexcept = default;

        /**
         * @brief Move Constructor
         *
         * Move Constructor of NonCopyable
         */
        NonCopyable(NonCopyable &&) noexcept = default;

        /**
         * @brief Move Assignement
         *
         * Move Assignement of NonCopyable
         */
        NonCopyable &operator=(NonCopyable &&) noexcept = default;

        /**
         * @brief Copy Constructor
         *
         * Copy Constructor of NonCopyable
         */
        NonCopyable(const NonCopyable &) noexcept = delete;

        /**
         * @brief Copy Assignement
         *
         * Copy Assignement of NonCopyable
         */
        void operator=(const NonCopyable &) noexcept = delete;
    };
} // namespace storm::core
