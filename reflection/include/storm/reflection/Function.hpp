#pragma once

#include <functional>
#include <storm/reflection/FunctionBase.hpp>

namespace storm::reflection {
    template<typename ReturnType, typename... Params>
    class Function;

    template<typename ReturnType, typename... Params>
    class Function<ReturnType(Params...)> final : public FunctionBase {
    public:
        using FunctionHandler = std::function<ReturnType(Params...)>;

        explicit Function(const std::string &name, FunctionHandler &&func)
              : m_name(name), m_func(std::move(func)) {}
        explicit Function(const std::string &name, const FunctionHandler &func)
              : m_name(name), m_func(func) {}
        explicit Function(std::string &&name, FunctionHandler &&func)
              : m_name(std::move(name)), m_func(std::move(func)) {}
        explicit Function(std::string &&name, const FunctionHandler &func)
              : m_name(std::move(name)), m_func(func) {}

        inline ReturnType operator()(Params &&... params) {
            return m_func(std::forward<Params>(params)...);
        }

        inline virtual const std::string &name() const noexcept override final { return m_name; }

    private:
        FunctionHandler m_func;
        std::string m_name;
    };
} // namespace storm::reflection
