#include <storm/switch-controller/Controller.hpp>
#include <storm/switch-controller/Constants.hpp>
#include <storm/switch-controller/Controller.hpp>

#include <array>
#include <cmath>
#include <algorithm>

using namespace storm::switchc;

Controller::Controller(ControllerType type)
	: m_type(type), m_rumble_data{0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40} {
	m_device.init();
}

Controller::~Controller() {

}

void Controller::initDevice() {
	std::vector<std::uint8_t> data;
	data.emplace_back(1);// ON

	sendSubCommand(RUMBLE_PLUS_SUBCOMMAND<std::uint8_t>, VIBRATION_CONTROL<std::uint8_t>, data);
	sendSubCommand(RUMBLE_PLUS_SUBCOMMAND<std::uint8_t>, IMU_DATA_CONTROL<std::uint8_t>, data);

	data.clear();
	data.emplace_back(0x31);

	sendSubCommand(RUMBLE_PLUS_SUBCOMMAND<std::uint8_t>, DATA_RATE_CONTROL<std::uint8_t>, data);

	setLED(PlayerLed::No_Player);
}

void Controller::connect() {
	std::uint16_t controller_type;
	switch(m_type) {
		case ControllerType::Left:
			controller_type = RIGHT_PRODUCT_HID<std::uint16_t>;
			break;
		case ControllerType::Right:
			controller_type = LEFT_PRODUCT_HID<std::uint16_t>;
			break;
		case ControllerType::Pro:
			controller_type = PRO_PRODUCT_HID<std::uint16_t>;
			break;
	}

	m_device.connect(COMMON_VENDOR_HID<std::uint16_t>,
					 controller_type);
}

void Controller::disconnect() {
	m_device.disconnect();
}

std::array<std::uint8_t, 34> Controller::getSubResponse(std::uint8_t subcommand) {
	std::vector<std::uint8_t> rep;
	std::array<std::uint8_t, 34> buff;
	do {
		rep = m_device.read(360);

		std::copy(std::begin(rep) + 14, std::begin(rep) + 14 + 34, std::begin(buff));

		if(rep[14] != subcommand) {
			m_reply_cache.emplace(rep[14], buff);
		}
	} while(rep[0] != 0x21);

	return buff;
}

void Controller::setLED(PlayerLed solid, PlayerLed flash) {
	std::vector<std::uint8_t> data;
	data.emplace_back(static_cast<std::uint8_t>(solid) | (static_cast<std::uint8_t>(flash) << 4));

	sendSubCommand(RUMBLE_PLUS_SUBCOMMAND<std::uint8_t>, LED_CONTROL<std::uint8_t>, data);
	auto rep = getSubResponse(LED_CONTROL<std::uint8_t>);
}

void Controller::setHomeLED() {
	std::vector<std::uint8_t> data;
	data.emplace_back(0xFF);

	sendSubCommand(RUMBLE_PLUS_SUBCOMMAND<std::uint8_t>, HOME_LED_CONTROL<std::uint8_t>, data);
	auto rep = getSubResponse(HOME_LED_CONTROL<std::uint8_t>);
}

void Controller::setRumbleFreq(float freq) {
	if(freq <= 0.f)
		return;
	auto encoded_freq = encodeFreq(freq);
	auto enc = encode(encoded_freq);

	std::copy(std::begin(enc), std::end(enc), std::begin(m_rumble_data));
	std::copy(std::begin(enc), std::end(enc), std::begin(m_rumble_data) + 4);

	sendCommand(RUMBLE_ONLY<std::uint8_t>, m_rumble_data);
}

Controller::DeviceInfo Controller::requestDeviceInfos() {
	if(!m_device_info) {
		m_device_info = std::make_unique<DeviceInfo>();

		sendSubCommand(RUMBLE_PLUS_SUBCOMMAND<std::uint8_t> ,DEVICE_INFO_REQUEST<std::uint8_t>, std::vector<std::uint8_t>{});
	}

	return *m_device_info;
}

std::pair<std::uint16_t, std::uint8_t> Controller::encodeFreq(float freq) {
	if(freq <= 0.f)
		freq = 0.f;
	else if (freq > 1252.f)
		freq = 1252.f;

	std::uint8_t encoded_hex_freq = static_cast<std::uint8_t>(std::round(std::log2(static_cast<double>(freq)/10.0)*32.0));

	std::uint16_t hf = (encoded_hex_freq-0x60) * 4;
	std::uint8_t lf = encoded_hex_freq-0x40;

	std::cout << std::hex << "0x" << int(hf) << " 0x" << int(lf) << std::endl;

	return {hf, lf};
}

std::array<std::uint8_t, 4> Controller::encode(std::pair<std::uint16_t, std::uint8_t> f) {
	std::array<std::uint8_t, 4> array;

	std::uint8_t hf_amp = 0xC8;

	array[0] = f.first & 0xFF;
	array[1] = hf_amp + ((f.first >> 8) & 0xFF);

	std::uint16_t lf_amp = 0x0072;

	array[2] = f.second + ((lf_amp >> 8) & 0xFF);
	array[3] = lf_amp & 0xFF;

	return array;
}
