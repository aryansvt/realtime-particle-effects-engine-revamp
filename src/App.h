#pragma once

struct GLFWwindow;

class App {
public:
    App(int width, int height, const char* title);
    ~App();

    void run();

private:
    void init_window(int width, int height, const char* title);
    void init_gl();
    void init_imgui();
    void shutdown_imgui();
    void apply_imgui_style();

    GLFWwindow* m_window{nullptr};
    int m_width{0};
    int m_height{0};
    bool m_running{true};
};
