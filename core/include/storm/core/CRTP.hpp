// Copryright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

/// \file core
/// \author Arthapz

#pragma once

/// \module core
namespace storm::core {

    /// Utility class for makeing CRTP
    template<typename T, template<typename> class crtp_type>
    struct CRTP {

        /// \group getters
        /// \return the underlying class type
        constexpr inline T &underlying() { return static_cast<T &>(*this); }

        /// \group getters
        /// \return the underlying class type
        constexpr inline T const &underlying() const { return static_cast<T const &>(*this); }
    };
} // namespace storm::core
