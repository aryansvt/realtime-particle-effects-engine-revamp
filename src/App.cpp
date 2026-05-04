#include "App.h"

#include "Camera.h"
#include "Scene.h"
#include "Particles.h"
#include "Presets.h"
#include "PostProcess.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

static void glfw_error(int code, const char* desc) {
    std::cerr << "glfw error " << code << " " << desc << "\n";
}

App::App(int width, int height, const char* title)
    : m_width(width), m_height(height) {
    init_window(width, height, title);
    init_gl();
    init_imgui();
    apply_imgui_style();
}

App::~App() {
    shutdown_imgui();

    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void App::init_window(int width, int height, const char* title) {
    glfwSetErrorCallback(glfw_error);
    if (!glfwInit()) {
        throw std::runtime_error("glfw init failed");
    }

    // request opengl 3 3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("window create failed");
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
}

void App::init_gl() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("glad load failed");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void App::shutdown_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void App::apply_imgui_style() {
    // custom dark style
    // a bit warmer than the default and rounded so panels feel less generic
    ImGuiStyle& s = ImGui::GetStyle();

    s.WindowRounding   = 6.0f;
    s.FrameRounding    = 4.0f;
    s.GrabRounding     = 4.0f;
    s.PopupRounding    = 4.0f;
    s.ScrollbarRounding= 6.0f;
    s.TabRounding      = 4.0f;

    s.WindowPadding    = ImVec2(12.0f, 10.0f);
    s.FramePadding     = ImVec2(8.0f, 4.0f);
    s.ItemSpacing      = ImVec2(8.0f, 6.0f);
    s.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
    s.ScrollbarSize    = 12.0f;
    s.GrabMinSize      = 10.0f;

    s.WindowBorderSize = 0.0f;
    s.FrameBorderSize  = 0.0f;

    ImVec4* c = s.Colors;
    const ImVec4 accent      = ImVec4(0.55f, 0.70f, 0.95f, 1.0f);
    const ImVec4 accent_soft = ImVec4(0.30f, 0.45f, 0.65f, 1.0f);

    c[ImGuiCol_WindowBg]          = ImVec4(0.08f, 0.09f, 0.11f, 0.96f);
    c[ImGuiCol_ChildBg]           = ImVec4(0.10f, 0.11f, 0.13f, 1.0f);
    c[ImGuiCol_PopupBg]           = ImVec4(0.09f, 0.10f, 0.12f, 0.98f);

    c[ImGuiCol_TitleBg]           = ImVec4(0.10f, 0.11f, 0.13f, 1.0f);
    c[ImGuiCol_TitleBgActive]     = ImVec4(0.13f, 0.14f, 0.18f, 1.0f);
    c[ImGuiCol_TitleBgCollapsed]  = ImVec4(0.10f, 0.11f, 0.13f, 0.7f);

    c[ImGuiCol_FrameBg]           = ImVec4(0.13f, 0.14f, 0.17f, 1.0f);
    c[ImGuiCol_FrameBgHovered]    = ImVec4(0.18f, 0.20f, 0.24f, 1.0f);
    c[ImGuiCol_FrameBgActive]     = ImVec4(0.22f, 0.25f, 0.30f, 1.0f);

    c[ImGuiCol_Header]            = ImVec4(0.18f, 0.20f, 0.25f, 1.0f);
    c[ImGuiCol_HeaderHovered]     = accent_soft;
    c[ImGuiCol_HeaderActive]      = accent;

    c[ImGuiCol_Button]            = ImVec4(0.16f, 0.18f, 0.22f, 1.0f);
    c[ImGuiCol_ButtonHovered]     = accent_soft;
    c[ImGuiCol_ButtonActive]      = accent;

    c[ImGuiCol_SliderGrab]        = accent;
    c[ImGuiCol_SliderGrabActive]  = ImVec4(0.70f, 0.85f, 1.0f, 1.0f);
    c[ImGuiCol_CheckMark]         = accent;

    c[ImGuiCol_Separator]         = ImVec4(0.22f, 0.24f, 0.28f, 1.0f);
    c[ImGuiCol_SeparatorHovered]  = accent_soft;
    c[ImGuiCol_SeparatorActive]   = accent;

    c[ImGuiCol_Text]              = ImVec4(0.90f, 0.92f, 0.95f, 1.0f);
    c[ImGuiCol_TextDisabled]      = ImVec4(0.45f, 0.48f, 0.52f, 1.0f);

    c[ImGuiCol_Border]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    c[ImGuiCol_ScrollbarBg]       = ImVec4(0.06f, 0.07f, 0.09f, 1.0f);
    c[ImGuiCol_ScrollbarGrab]     = ImVec4(0.20f, 0.22f, 0.26f, 1.0f);
    c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.33f, 0.38f, 1.0f);
    c[ImGuiCol_ScrollbarGrabActive]  = accent_soft;
}

void App::run() {
    Camera cam((float)m_width / (float)m_height);
    Scene scene;
    scene.init();

    ParticleSystem particles;
    particles.init(60000);

    PostProcess post;
    int fb_w, fb_h;
    glfwGetFramebufferSize(m_window, &fb_w, &fb_h);
    post.init(fb_w, fb_h);

    auto presets = make_presets();
    int preset_index = 0;
    particles.set_preset(presets[preset_index]);

    // keep editable settings alive across frames
    EmitterSettings current = presets[preset_index];

    bool pause = false;
    bool right_mouse_look = false;
    bool live_update = true;

    double last_x = 0.0;
    double last_y = 0.0;
    bool mouse_init = false;

    int last_fb_w = fb_w;
    int last_fb_h = fb_h;

    float last_time = (float)glfwGetTime();

    while (!glfwWindowShouldClose(m_window) && m_running) {
        glfwPollEvents();

        const float now = (float)glfwGetTime();
        float dt = now - last_time;
        last_time = now;
        if (dt > 0.1f) dt = 0.1f;

        int w, h;
        glfwGetFramebufferSize(m_window, &w, &h);
        if (w <= 0 || h <= 0) {
            // window minimized  skip frame
            continue;
        }
        cam.set_aspect((float)w / (float)h);

        // resize fbos if the window changed
        if (w != last_fb_w || h != last_fb_h) {
            post.resize(w, h);
            last_fb_w = w;
            last_fb_h = h;
        }

        // start imgui frame early so io flags are current
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        const bool capture_keyboard = io.WantCaptureKeyboard;
        const bool capture_mouse = io.WantCaptureMouse;

        // camera input
        glm::vec3 move{ 0.0f };
        if (!capture_keyboard) {
            if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) move.z += 1.0f;
            if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) move.z -= 1.0f;
            if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) move.x -= 1.0f;
            if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) move.x += 1.0f;
            if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) move.y += 1.0f;
            if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) move.y -= 1.0f;
        }

        right_mouse_look =
            !capture_mouse &&
            glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

        if (right_mouse_look) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double mx, my;
            glfwGetCursorPos(m_window, &mx, &my);
            if (!mouse_init) {
                last_x = mx;
                last_y = my;
                mouse_init = true;
            }
            cam.on_mouse_delta((float)(mx - last_x), (float)(my - last_y));
            last_x = mx;
            last_y = my;
        } else {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouse_init = false;
        }

        if (glm::length(move) > 0.001f) {
            cam.move_local(move, dt);
        }

        // preset hotkeys 1 through 7
        if (!capture_keyboard) {
            const int hot_keys[] = {
                GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4,
                GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7
            };
            const int max_idx = (std::min)((int)presets.size(), 7);
            for (int i = 0; i < max_idx; i++) {
                if (glfwGetKey(m_window, hot_keys[i]) == GLFW_PRESS && preset_index != i) {
                    preset_index = i;
                    current = presets[preset_index];
                    particles.set_preset(current);
                }
            }

            if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                particles.request_burst(350);
            }
            if (glfwGetKey(m_window, GLFW_KEY_P) == GLFW_PRESS) pause = true;
            if (glfwGetKey(m_window, GLFW_KEY_O) == GLFW_PRESS) pause = false;
        }

        // step simulation
        if (!pause) {
            particles.update(dt, cam);
        }

        // ---- render to hdr fbo ----
        post.begin_scene_pass();

        scene.render(cam.view(), cam.projection());

        // particles read the same depth attachment for soft particle fade
        // depth writes are off so this is a read only feedback that drivers handle fine
        particles.render(cam, post.scene_depth_texture(), w, h);

        // ---- bloom and tonemap to default backbuffer ----
        post.end_and_present(w, h);

        // ---- ui ----
        ImGui::SetNextWindowSize(ImVec2(360.0f, 0.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(12.0f, 12.0f), ImGuiCond_FirstUseEver);
        ImGui::Begin("particle lab");

        // header stats line
        ImGui::Text("%.0f fps   alive %d / %d", io.Framerate, particles.alive_count(), particles.max_count());

        ImGui::Separator();

        // preset chooser as a button grid plus a combo for keyboard friendliness
        ImGui::Text("preset");
        const float avail = ImGui::GetContentRegionAvail().x;
        const int per_row = 4;
        const float btn_w = (avail - (per_row - 1) * 6.0f) / (float)per_row;

        for (int i = 0; i < (int)presets.size(); i++) {
            if (i % per_row != 0) ImGui::SameLine(0.0f, 6.0f);

            const bool is_active = (i == preset_index);
            if (is_active) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.70f, 0.95f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.80f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.05f, 0.05f, 0.07f, 1.0f));
            }

            if (ImGui::Button(presets[i].name.c_str(), ImVec2(btn_w, 0.0f))) {
                preset_index = i;
                current = presets[preset_index];
                particles.set_preset(current);
            }

            if (is_active) {
                ImGui::PopStyleColor(3);
            }
        }

        ImGui::Spacing();
        ImGui::Checkbox("live update", &live_update);
        ImGui::SameLine();
        if (ImGui::Button("burst")) {
            particles.request_burst(1200);
        }
        ImGui::SameLine();
        if (ImGui::Button(pause ? "resume" : "pause")) {
            pause = !pause;
        }
        ImGui::SameLine();
        if (ImGui::Button("reset")) {
            current = make_presets()[preset_index];
            presets[preset_index] = current;
            particles.set_preset(current);
        }

        bool changed = false;

        // emitter
        if (ImGui::CollapsingHeader("emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
            changed |= ImGui::SliderFloat3("position", &current.position.x, -15.0f, 15.0f);
            changed |= ImGui::SliderFloat3("position jitter", &current.position_jitter.x, 0.0f, 30.0f);
            changed |= ImGui::SliderFloat("spawn rate", &current.spawn_rate, 0.0f, 8000.0f);
            changed |= ImGui::SliderFloat("life min", &current.life_min, 0.05f, 12.0f);
            changed |= ImGui::SliderFloat("life max", &current.life_max, 0.05f, 16.0f);
        }

        // motion
        if (ImGui::CollapsingHeader("motion")) {
            changed |= ImGui::SliderFloat3("base velocity", &current.velocity.x, -25.0f, 25.0f);
            changed |= ImGui::SliderFloat3("velocity jitter", &current.velocity_jitter.x, 0.0f, 10.0f);
            changed |= ImGui::SliderFloat3("acceleration", &current.acceleration.x, -40.0f, 40.0f);
            changed |= ImGui::SliderFloat3("wind", &current.wind.x, -6.0f, 6.0f);
            changed |= ImGui::SliderFloat("damping", &current.damping, 0.0f, 1.0f);
        }

        // appearance
        if (ImGui::CollapsingHeader("appearance")) {
            changed |= ImGui::SliderFloat("size start", &current.size_start, 0.01f, 3.0f);
            changed |= ImGui::SliderFloat("size end",   &current.size_end,   0.01f, 4.0f);
            changed |= ImGui::ColorEdit4("color start", &current.color_start.x);
            changed |= ImGui::ColorEdit4("color end",   &current.color_end.x);
            changed |= ImGui::SliderFloat("emissive",   &current.emissive,   0.0f, 4.0f);

            int blend = (current.blend == BlendMode::Additive) ? 1 : 0;
            if (ImGui::RadioButton("alpha blend", blend == 0)) { blend = 0; changed = true; }
            ImGui::SameLine();
            if (ImGui::RadioButton("additive",    blend == 1)) { blend = 1; changed = true; }
            current.blend = (blend == 1) ? BlendMode::Additive : BlendMode::Alpha;

            changed |= ImGui::Checkbox("depth sort", &current.depth_sort);
            changed |= ImGui::Checkbox("align rotation to velocity", &current.align_to_velocity);
        }

        // advanced features beyond class material
        if (ImGui::CollapsingHeader("advanced")) {
            ImGui::TextDisabled("curl noise turbulence");
            changed |= ImGui::SliderFloat("curl strength",   &current.curl_strength,   0.0f, 5.0f);
            changed |= ImGui::SliderFloat("curl scale",      &current.curl_scale,      0.05f, 3.0f);
            changed |= ImGui::SliderFloat("curl time scale", &current.curl_time_scale, 0.0f, 3.0f);

            ImGui::Spacing();
            ImGui::TextDisabled("soft particles");
            changed |= ImGui::SliderFloat("soft fade dist", &current.soft_fade, 0.0f, 3.0f);

            ImGui::Spacing();
            ImGui::TextDisabled("velocity stretched billboard");
            changed |= ImGui::SliderFloat("velocity stretch", &current.velocity_stretch, 0.0f, 0.30f);
        }

        // post processing settings  these live on the post object not the preset
        if (ImGui::CollapsingHeader("post process")) {
            ImGui::Checkbox("bloom", &post.bloom_enabled);
            ImGui::SliderFloat("bloom threshold", &post.bloom_threshold, 0.0f, 4.0f);
            ImGui::SliderFloat("bloom strength",  &post.bloom_strength,  0.0f, 2.0f);
            ImGui::SliderInt  ("blur iterations", &post.blur_iterations, 1, 10);
            ImGui::SliderFloat("exposure",        &post.exposure,        0.1f, 4.0f);
        }

        // controls hint
        if (ImGui::CollapsingHeader("controls")) {
            ImGui::BulletText("wasd to move  q e for up down");
            ImGui::BulletText("hold right mouse to look");
            ImGui::BulletText("1 to 7 swap presets");
            ImGui::BulletText("space adds a burst");
            ImGui::BulletText("p pause  o resume");
        }

        if (live_update && changed) {
            presets[preset_index] = current;
            particles.set_preset(current);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }

    post.shutdown();
    particles.shutdown();
    scene.shutdown();
}
