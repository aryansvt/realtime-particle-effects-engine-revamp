#include "App.h"

#include <iostream>

int main() {
    try {
        App app(1280, 720, "Real-Time Particle Effects Engine");
        app.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "fatal " << e.what() << "\n";
        return 1;
    }
}
