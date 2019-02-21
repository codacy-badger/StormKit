#include <storm/log/LogOutput.hpp>
#include <storm/window/Window.hpp>

int main() {
    using namespace storm::window;

    storm::InstallDefaultLogOutput();

    auto window = Window("Hello world", {800u, 600u}, WindowStyle::Close);

    while(window.isOpen()) {
        Event event;
        while(window.waitEvent(event)) {
            if(
              event.type == EventType::Closed ||
              (event.type == EventType::KeyPressed && event.keyEvent.key == Key::ESCAPE))
                window.close();
            else if(event.type == EventType::KeyPressed) {
                const auto key = event.keyEvent.key;
                storm::ILOG("Key pressed: %{1}", key);
            }
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
