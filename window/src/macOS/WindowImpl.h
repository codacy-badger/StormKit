#pragma once

#include <storm/window/AbstractWindow.hpp>
#include <storm/core/Platform.hpp>

#ifdef __OBJC__
@class StormWindowController;
using StormWindowControllerPtr = StormWindowController*;
#else
using StormWindowControllerPtr = void*;
#endif

class ContextImpl;
class STORM_PRIVATE WindowImpl : public storm::window::AbstractWindow {
	public:
		WindowImpl() noexcept;
		WindowImpl(const std::string &title, const storm::window::VideoSettings &settings, storm::window::WindowStyle style) noexcept;
		~WindowImpl() override;

		void create(const std::string &title, const storm::window::VideoSettings &settings, storm::window::WindowStyle style) noexcept override;
		void close() noexcept override;
		void display() noexcept override;

		//BLC
		bool pollEvent(storm::window::Event &event, void *native_event) noexcept override;
		void waitEvent() noexcept override;

		void setTitle(const std::string &title)              noexcept override;
		void setVideoSettings(const storm::window::VideoSettings &settings) noexcept override;

		storm::window::VideoSettings::Size size() const noexcept override;

		bool isOpen()    const noexcept override;
		bool isVisible() const noexcept override;

		void setMousePosition(std::int16_t x, std::int16_t y) const noexcept override;

		//ContextImpl &context() noexcept override { return *m_context; }

		storm::window::NativeHandle nativeHandle() const noexcept override;
	private:
		void initCocoaProcess();

		StormWindowControllerPtr m_controller;
};
