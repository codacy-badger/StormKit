#pragma once

#include <string>
#include <queue>

#include <storm/window/VideoSettings.hpp>
#include <storm/window/WindowStyle.hpp>
#include <storm/window/Event.hpp>
#include <storm/window/MouseButton.hpp>
#include <storm/window/Key.hpp>

namespace storm::window {
	using NativeHandle = void*;
	struct Event;
	class AbstractWindow {
		public:
			AbstractWindow();
			virtual ~AbstractWindow();

			virtual void create(const std::string &title, const VideoSettings &settings, WindowStyle style) = 0;
			virtual void close() noexcept = 0;
			virtual void display() noexcept = 0;

			//BLC
			virtual bool pollEvent(Event &event, void *native_event = nullptr) noexcept;
			virtual bool waitEvent(Event &event, void *native_event = nullptr) noexcept = 0;

			virtual void setTitle(const std::string &title)              noexcept = 0;
			virtual void setVideoSettings(const VideoSettings &settings) noexcept = 0;

			virtual VideoSettings::Size size() const noexcept = 0;

			inline const std::string &title()           const noexcept  { return m_title; }
			inline const VideoSettings &videoSettings() const noexcept  { return m_video_settings; }
			virtual bool isOpen()                       const noexcept = 0;
			virtual bool isVisible()                    const noexcept = 0;

			virtual void setMousePosition(std::int16_t x, std::int16_t y) const noexcept = 0;

			virtual NativeHandle nativeHandle() const noexcept = 0;

			void mouseDownEvent(MouseButton button, std::int16_t x, std::int16_t y) noexcept;
			void mouseUpEvent(MouseButton button, std::int16_t x, std::int16_t y) noexcept;

			void mouseMoveEvent(std::int16_t x, std::int16_t y) noexcept;

			void mouseEnteredEvent() noexcept;
			void mouseExitedEvent() noexcept;

			void keyDownEvent(Key key) noexcept;
			void keyUpEvent(Key key) noexcept;

			void closeEvent() noexcept;

			void resizeEvent(std::uint16_t width, std::uint16_t height) noexcept;
			void minimizeEvent() noexcept;
			void maximizeEvent() noexcept;
		protected:
			void pushEvent(Event event) noexcept;

			std::string     m_title;
			VideoSettings   m_video_settings;
			WindowStyle     m_style;

			std::queue<Event> m_events;
	};
}
