#pragma once
#include <cstdint>
#include <storm/core/Flags.hpp>

namespace storm::switchc {
    template<typename T>
    constexpr static T COMMON_VENDOR_HID = T(0x057e);

    template<typename T>
    constexpr static T RIGHT_PRODUCT_HID = T(0x2007);

    template<typename T>
    constexpr static T LEFT_PRODUCT_HID = T(0x2006);

    template<typename T>
    constexpr static T PRO_PRODUCT_HID = T(0x2009);

    template<typename T>
    constexpr static T RUMBLE_PLUS_SUBCOMMAND = T(0x1);

    template<typename T>
    constexpr static T RUMBLE_ONLY = T(0x10);

    template<typename T>
    constexpr static T VIBRATION_CONTROL = T(0x48);

    template<typename T>
    constexpr static T IMU_DATA_CONTROL = T(0x40);

    template<typename T>
    constexpr static T DATA_RATE_CONTROL = T(03);

    template<typename T>
    constexpr static T LED_CONTROL = T(0x30);

    template<typename T>
    constexpr static T HOME_LED_CONTROL = T(0x38);

    template<typename T>
    constexpr static T DEVICE_INFO_REQUEST = T(0x02);

    enum class PlayerLed {
        No_Player    = 0,
        Player_One   = 1,
        Player_Two   = 2,
        Player_Three = 4,
        Player_Four  = 8
    };
} // namespace storm::switchc

FLAG_ENUM(storm::switchc::PlayerLed)
