#pragma once

#include <storm/core/Platform.hpp>
#include <storm/core/Memory.hpp>
#include <storm/core/Flags.hpp>
#include <storm/core/NonCopyable.hpp>

#include <storm/window/AbstractWindow.hpp>

namespace storm::window {
	class AbstractWindow;
	class Window : public core::NonCopyable {
		public:
			Window() noexcept;
			Window(const std::string &title, const VideoSettings &settings, WindowStyle style) noexcept;
			~Window();

			Window(Window&&);
			Window &operator=(Window&&);

			inline void create(const std::string &title, const VideoSettings &settings, WindowStyle style) noexcept { m_impl->create(title, settings, style); }
			inline void close() noexcept { m_impl->close(); }
			void display() noexcept;

			inline bool pollEvent(Event &event, void *native_event = nullptr) noexcept { return m_impl->pollEvent(event, native_event); }
			inline bool waitEvent(Event &event, void *native_event = nullptr) noexcept { return m_impl->waitEvent(event, native_event); }

			inline void setTitle(const std::string &title) noexcept { m_impl->setTitle(title); }
			inline void setVideoSettings(const VideoSettings &settings) noexcept  { m_impl->setVideoSettings(settings); }

			inline VideoSettings::Size size() const noexcept { return m_impl->size(); }

			inline const std::string &title()    const noexcept { return m_impl->title(); }
			inline VideoSettings videoSettings() const noexcept { return m_impl->videoSettings(); }
			inline bool isOpen()                 const noexcept { return m_impl->isOpen(); }
			inline bool isVisible()              const noexcept { return m_impl->isVisible(); }

			inline void setMousePosition(std::int16_t x, std::int16_t y) const noexcept { m_impl->setMousePosition(x, y); }

			inline NativeHandle nativeHandle() const noexcept { return m_impl->nativeHandle(); }
		private:
			std::unique_ptr<AbstractWindow> m_impl;
	};
}
