#pragma once
#include <array>
#include <storm/switch-controller/Constants.hpp>
#include <storm/switch-controller/Controller.hpp>
#include <storm/switch-controller/HIDDevice.hpp>
#include <unordered_map>
#include <vector>

namespace storm::switchc {
    class Controller {
    public:
        enum class ControllerType { Right, Left, Pro };

        struct DeviceInfo {
            std::uint8_t firmware_major;
            std::uint8_t firmware_minor;
            std::uint8_t controller_type;
            std::uint8_t dummy;
            std::uint8_t mac[ 6 ];
            std::uint8_t dummy2;
            std::uint8_t spi;
        };

        explicit Controller(ControllerType type);
        ~Controller();

        void initDevice();
        void connect();
        void disconnect();

        void llol();

        template<typename T>
        void sendCommand(std::uint8_t command, T data) {
            std::vector<std::uint8_t> buff;

            buff.emplace_back(command);
            if(data.size()) std::copy(std::begin(data), std::end(data), std::back_inserter(buff));

            m_device.write(buff);

            auto readed = m_device.read();
        }

        template<typename T>
        void sendSubCommand(std::uint8_t command, std::uint8_t subcommand, T &&data) {
            std::vector<std::uint8_t> buff;
            buff.emplace_back(++m_global_counter & 0xF);

            std::copy(std::begin(m_rumble_data), std::end(m_rumble_data), std::back_inserter(buff));

            buff.emplace_back(subcommand);
            if(data.size()) std::copy(std::begin(data), std::end(data), std::back_inserter(buff));

            sendCommand(command, buff);
        }

        std::array<std::uint8_t, 34> getSubResponse(std::uint8_t subcommand);

        void setLED(PlayerLed solid, PlayerLed flash = PlayerLed::No_Player);
        void setHomeLED();
        void setRumbleFreq(float freq);

        DeviceInfo requestDeviceInfos();

    private:
        std::pair<std::uint16_t, std::uint8_t> encodeFreq(float freq);
        std::array<std::uint8_t, 4> encode(std::pair<std::uint16_t, std::uint8_t> f);

        HIDDevice m_device;
        ControllerType m_type;

        std::uint8_t m_global_counter;

        std::array<std::uint8_t, 8> m_rumble_data;
        std::unique_ptr<DeviceInfo> m_device_info;

        std::unordered_map<std::uint8_t, std::array<std::uint8_t, 34>> m_reply_cache;
    };
} // namespace storm::switchc
