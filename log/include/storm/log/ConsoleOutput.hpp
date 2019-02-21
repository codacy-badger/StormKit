#pragma once

/**
 * @file ConsoleOutput.hpp
 * @brief Console Output class
 * @author Arthapz
 * @version 0.1
 **/

#include <storm/log/LogOutput.hpp>

namespace storm::log {
    /**
     * @class ConsoleLog
     * @ingroup Log
     * @brief Console Logger
     *
     * The class allow logging to the standart output
     */
    class STORM_PUBLIC ConsoleOutput final : public LogOutput {
    public:
        /**
         * @brief Constructor
         *
         * Default Constructor of ConsoleOutput
         */
        explicit ConsoleOutput() = default;

        /**
         * @brief Destructor
         *
         * Destructor of ConsoleOutput
         */
        ~ConsoleOutput() override = default;

    private:
        void write(const char *string) override;
    };
} // namespace storm::log
