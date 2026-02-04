#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>

void moveMouse(int dx, int dy) {
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput(1, &input, sizeof(INPUT));
}

void jiggleMouse() {
    // Small movement pattern: right, down, left, up (returns to original position)
    moveMouse(10, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    moveMouse(0, 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    moveMouse(-10, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    moveMouse(0, -10);
}

int main() {
    std::cout << "Mouse Jiggler Started!\n";
    std::cout << "Press Ctrl+C to stop.\n";
    std::cout << "Jiggling every second...\n\n";

    const auto interval = std::chrono::seconds(1);

    while (true) {
        jiggleMouse();
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(interval);
    }

    return 0;
}
