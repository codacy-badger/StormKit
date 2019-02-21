#include <array>
#include <storm/log/LogOutput.hpp>
#include <storm/switch-controller/HIDDevice.hpp>

using namespace storm::switchc;

bool HIDDevice::m_initialised = false;
HIDDevice::HIDDevice() : m_handle(nullptr) {
}

HIDDevice::~HIDDevice() {
    disconnect();
}

void HIDDevice::init() {
    if(!m_initialised) {
        hid_init();
        m_initialised = true;
    }
}

void HIDDevice::connect(std::uint16_t vendor_id, std::uint16_t product_id) {
    m_handle = hid_open(vendor_id, product_id, nullptr);

    if(m_handle)
        storm::DLOG("Connection to %1 %2 device done", vendor_id, product_id);
    else
        storm::DLOG("Connection to %1 %2 device failed !", vendor_id, product_id);
}

void HIDDevice::disconnect() {
    hid_close(m_handle);
    m_handle = nullptr;
}

std::vector<std::uint8_t> HIDDevice::read() {
    std::array<std::uint8_t, 1024> buff;

    auto res = hid_read(m_handle, buff.data(), buff.size());

    if(res == -1) return {};

    std::vector<std::uint8_t> vec;
    vec.reserve(res);
    std::copy(std::begin(buff), std::end(buff), std::back_inserter(vec));

    return vec;
}

std::vector<std::uint8_t> HIDDevice::read(std::size_t size) {
    std::vector<std::uint8_t> buff;
    buff.resize(size);

    auto res = hid_read(m_handle, buff.data(), buff.size());

    if(res == -1) return {};

    return buff;
}
