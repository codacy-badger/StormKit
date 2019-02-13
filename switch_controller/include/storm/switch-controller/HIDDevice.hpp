#pragma once
#include <cstdint>
#include <hidapi/hidapi.h>
#include <storm/core/Assert.hpp>
#include <storm/core/Memory.hpp>
#include <storm/log/LogOutput.hpp>
#include <vector>

namespace storm::switchc {
	class HIDDevice {
	public:
		explicit HIDDevice();
		~HIDDevice();

		static void init();

		void connect(std::uint16_t vendor_id, std::uint16_t product_id);
		void disconnect();

		template <typename T>
		void write(T &&array) {
			ASSERT(m_handle, "can't send data to null handle");

			auto res = hid_write(
			    m_handle, &(array.front()), storm::core::size(array));

#ifndef NDEBUG
			if (res != -1)
				storm::DLOG("%1 bits writed on %2", res, m_handle);
			else
				storm::DLOG("Failed to write %1 bits on %2",
				    storm::core::size(array), m_handle);
#endif
		}

		std::vector<std::uint8_t> read();
		std::vector<std::uint8_t> read(std::size_t size);

	private:
		static bool m_initialised;
		hid_device *m_handle;
	};
}
