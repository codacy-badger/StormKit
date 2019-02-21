#pragma once

#include <functional>
#include <memory>
#include <storm/core/Platform.hpp>
#include <storm/window/AbstractWindow.hpp>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

namespace storm::window {
    class WindowImpl : public AbstractWindow {
        struct XCBConnectionDeleter {
            void operator()(xcb_connection_t *connection) { xcb_disconnect(connection); }
        };

        using XCBScopedConnection = std::unique_ptr<xcb_connection_t, XCBConnectionDeleter>;

    public:
        WindowImpl();
        WindowImpl(
          const std::string &title,
          const storm::window::VideoSettings &settings,
          storm::window::WindowStyle style);
        ~WindowImpl() override;

        void create(
          const std::string &title,
          const storm::window::VideoSettings &settings,
          storm::window::WindowStyle style) override;
        void close() noexcept override;
        void display() noexcept override;

        // BLC
        bool pollEvent(storm::window::Event &event, void *native_event) noexcept override;
        bool waitEvent(Event &event, void *native_event) noexcept override;

        void setTitle(const std::string &title) noexcept override;
        void setVideoSettings(const storm::window::VideoSettings &settings) noexcept override;

        VideoSettings::Size size() const noexcept override;

        bool isOpen() const noexcept override;
        bool isVisible() const noexcept override;

        void setMousePosition(std::int16_t x, std::int16_t y) const noexcept override;

        storm::window::NativeHandle nativeHandle() const noexcept override;

    private:
        void processEvents(xcb_generic_event_t xevent, void *native_event);

        XCBScopedConnection m_connection;
        xcb_window_t m_window;

        struct Handles {
            xcb_connection_t *connection;
            xcb_window_t window;
        } handles;

        xcb_key_symbols_t *m_key_symbols;

        bool m_is_open;
        bool m_is_visible;

        mutable std::int16_t m_mouse_x;
        mutable std::int16_t m_mouse_y;
    };
} // namespace storm::window
