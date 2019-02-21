#pragma once
#include <storm/core/Configure.hpp>

#if __has_include(<any>)
#    include <any>
namespace _std {
    using any = std::any;
}
#elif __has_include(<experimental/any>)
#    include <experimental/any>
namespace _std {
    using any = std::experimental::any;
}
#endif
