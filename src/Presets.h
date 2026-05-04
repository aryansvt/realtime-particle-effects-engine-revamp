#pragma once

#include "Particles.h"
#include <vector>

inline std::vector<EmitterSettings> make_presets() {
    std::vector<EmitterSettings> presets;

    {
        EmitterSettings p;
        p.name = "fire";
        p.blend = BlendMode::Additive;
        p.spawn_rate = 900.0f;
        p.position = {0.0f, 0.05f, 0.0f};
        p.position_jitter = {0.22f, 0.02f, 0.22f};
        p.velocity = {0.0f, 2.8f, 0.0f};
        p.velocity_jitter = {1.3f, 0.7f, 1.3f};
        p.acceleration = {0.0f, 0.2f, 0.0f};
        p.wind = {0.15f, 0.0f, 0.0f};
        p.damping = 0.12f;
        p.color_start = {1.0f, 0.75f, 0.25f, 0.85f};
        p.color_end = {0.95f, 0.1f, 0.02f, 0.0f};
        p.size_start = 0.14f;
        p.size_end = 0.55f;
        p.life_min = 0.45f;
        p.life_max = 1.05f;
        p.texture_tile = 0;
        p.depth_sort = false;
        // curl noise gives the flame natural turbulence instead of clean arcs
        p.curl_strength = 1.4f;
        p.curl_scale = 0.9f;
        p.curl_time_scale = 0.7f;
        // emissive over 1 pushes the brightest pixels into the bloom pass
        p.emissive = 1.6f;
        presets.push_back(p);
    }

    {
        EmitterSettings p;
        p.name = "smoke";
        p.blend = BlendMode::Alpha;
        p.spawn_rate = 450.0f;
        p.position = {0.0f, 0.05f, 0.0f};
        p.position_jitter = {0.30f, 0.03f, 0.30f};
        p.velocity = {0.0f, 1.35f, 0.0f};
        p.velocity_jitter = {0.55f, 0.35f, 0.55f};
        p.acceleration = {0.0f, 0.05f, 0.0f};
        p.wind = {0.25f, 0.0f, 0.05f};
        p.damping = 0.06f;
        p.color_start = {0.55f, 0.55f, 0.55f, 0.65f};
        p.color_end = {0.28f, 0.28f, 0.28f, 0.0f};
        p.size_start = 0.25f;
        p.size_end = 1.25f;
        p.life_min = 1.3f;
        p.life_max = 2.5f;
        p.texture_tile = 1;
        p.depth_sort = true;
        // heavy curl noise so smoke billows instead of marching upward
        p.curl_strength = 2.4f;
        p.curl_scale = 0.5f;
        p.curl_time_scale = 0.4f;
        // soft fade hides the harsh clip where smoke meets the ground
        p.soft_fade = 0.6f;
        presets.push_back(p);
    }

    {
        EmitterSettings p;
        p.name = "sparks";
        p.blend = BlendMode::Additive;
        p.spawn_rate = 0.0f;
        p.burst_count = 1200;
        p.position = {0.0f, 0.15f, 0.0f};
        p.position_jitter = {0.08f, 0.08f, 0.08f};
        p.velocity = {0.0f, 5.2f, 0.0f};
        p.velocity_jitter = {7.0f, 4.0f, 7.0f};
        p.acceleration = {0.0f, -9.8f, 0.0f};
        p.wind = {0.0f, 0.0f, 0.0f};
        p.damping = 0.0f;
        p.color_start = {1.0f, 0.95f, 0.55f, 0.9f};
        p.color_end = {1.0f, 0.35f, 0.1f, 0.0f};
        p.size_start = 0.06f;
        p.size_end = 0.02f;
        p.life_min = 0.35f;
        p.life_max = 0.85f;
        p.texture_tile = 2;
        p.depth_sort = false;
        p.align_to_velocity = false; // velocity stretch handles this better
        // anisotropic billboard turns each spark into a streak along its motion
        p.velocity_stretch = 0.08f;
        // strong emissive so sparks blow out into bright bloom trails
        p.emissive = 2.5f;
        presets.push_back(p);
    }

    {
        EmitterSettings p;
        p.name = "rain";
        p.blend = BlendMode::Alpha;
        p.spawn_rate = 4200.0f;
        p.position = {0.0f, 10.0f, 0.0f};
        p.position_jitter = {24.0f, 0.2f, 24.0f};
        p.velocity = {0.0f, -18.0f, 0.0f};
        p.velocity_jitter = {1.2f, 3.0f, 1.2f};
        p.acceleration = {0.0f, -2.5f, 0.0f};
        p.wind = {1.15f, 0.0f, 0.30f};
        p.damping = 0.0f;
        p.color_start = {0.72f, 0.86f, 1.0f, 0.80f};
        p.color_end = {0.72f, 0.86f, 1.0f, 0.0f};
        p.size_start = 0.24f;
        p.size_end = 0.12f;
        p.life_min = 0.65f;
        p.life_max = 1.20f;
        p.texture_tile = 3;
        p.depth_sort = false;
        p.align_to_velocity = false;
        // slight stretch so fast drops elongate rather than look like floating blobs
        p.velocity_stretch = 0.04f;
        presets.push_back(p);
    }

    {
        EmitterSettings p;
        p.name = "magic";
        p.blend = BlendMode::Additive;
        p.spawn_rate = 650.0f;
        p.position = {0.0f, 0.2f, 0.0f};
        p.position_jitter = {0.6f, 0.2f, 0.6f};
        p.velocity = {0.0f, 1.5f, 0.0f};
        p.velocity_jitter = {2.2f, 1.6f, 2.2f};
        p.acceleration = {0.0f, 0.0f, 0.0f};
        p.wind = {0.0f, 0.0f, 0.0f};
        p.damping = 0.04f;
        p.color_start = {0.75f, 0.35f, 1.0f, 0.7f};
        p.color_end = {0.25f, 0.95f, 0.95f, 0.0f};
        p.size_start = 0.10f;
        p.size_end = 0.60f;
        p.life_min = 0.8f;
        p.life_max = 1.8f;
        p.texture_tile = 4;
        p.depth_sort = false;
        p.curl_strength = 0.8f;
        p.curl_scale = 0.7f;
        p.emissive = 1.8f;
        presets.push_back(p);
    }

    // ------- new presets that lean on curl noise and soft particles -------

    {
        // glowing embers drifting upward in a slow turbulent field
        // showcases curl noise the most clearly because the dots trace the field
        EmitterSettings p;
        p.name = "embers";
        p.blend = BlendMode::Additive;
        p.spawn_rate = 120.0f;
        p.position = {0.0f, 0.5f, 0.0f};
        p.position_jitter = {3.5f, 1.2f, 3.5f};
        p.velocity = {0.0f, 0.45f, 0.0f};
        p.velocity_jitter = {0.25f, 0.30f, 0.25f};
        p.acceleration = {0.0f, 0.25f, 0.0f};
        p.wind = {0.05f, 0.0f, 0.05f};
        p.damping = 0.30f;
        p.color_start = {1.0f, 0.65f, 0.20f, 0.90f};
        p.color_end = {1.0f, 0.20f, 0.05f, 0.0f};
        p.size_start = 0.05f;
        p.size_end = 0.10f;
        p.life_min = 2.5f;
        p.life_max = 5.0f;
        p.texture_tile = 5; // soft round
        p.depth_sort = false;
        p.curl_strength = 2.0f;
        p.curl_scale = 0.6f;
        p.curl_time_scale = 0.3f;
        p.emissive = 2.2f;
        presets.push_back(p);
    }

    {
        // gentle drifting snow with a curl noise sway and soft ground fade
        // showcases soft particles cleanly because flakes pass close to the floor
        EmitterSettings p;
        p.name = "snow";
        p.blend = BlendMode::Alpha;
        p.spawn_rate = 900.0f;
        p.position = {0.0f, 12.0f, 0.0f};
        p.position_jitter = {28.0f, 0.5f, 28.0f};
        p.velocity = {0.0f, -1.4f, 0.0f};
        p.velocity_jitter = {0.4f, 0.3f, 0.4f};
        p.acceleration = {0.0f, -0.4f, 0.0f};
        p.wind = {0.20f, 0.0f, 0.10f};
        p.damping = 0.45f;
        p.color_start = {0.95f, 0.97f, 1.00f, 0.85f};
        p.color_end = {0.95f, 0.97f, 1.00f, 0.0f};
        p.size_start = 0.07f;
        p.size_end = 0.05f;
        p.life_min = 7.0f;
        p.life_max = 14.0f;
        p.texture_tile = 5; // soft round
        p.depth_sort = true;
        p.curl_strength = 0.6f;
        p.curl_scale = 0.35f;
        p.curl_time_scale = 0.25f;
        p.soft_fade = 0.4f;
        presets.push_back(p);
    }

    return presets;
}
