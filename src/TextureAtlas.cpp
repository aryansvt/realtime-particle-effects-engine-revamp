#include "TextureAtlas.h"

#include <cmath>
#include <algorithm>

#include <glad/glad.h>

static unsigned char uc(float x) {
    x = (std::max)(0.0f, (std::min)(1.0f, x));
    return (unsigned char)(x * 255.0f + 0.5f);
}

TextureAtlas::~TextureAtlas() {
    destroy();
}

void TextureAtlas::destroy() {
    if (m_tex) {
        glDeleteTextures(1, &m_tex);
        m_tex = 0;
    }
}

static void put_rgba(std::vector<unsigned char>& img, int w, int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    const int idx = (y * w + x) * 4;
    img[idx + 0] = r;
    img[idx + 1] = g;
    img[idx + 2] = b;
    img[idx + 3] = a;
}

std::vector<unsigned char> TextureAtlas::make_tile_fire(int size) const {
    std::vector<unsigned char> img(size * size * 4, 0);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            float nx = (x + 0.5f) / size * 2.0f - 1.0f;
            float ny = (y + 0.5f) / size * 2.0f - 1.0f;
            float r = std::sqrt(nx * nx + ny * ny);
            float falloff = std::exp(-r * 3.2f);
            float core = std::exp(-r * 10.0f);
            float t = 1.0f - (float)y / (float)size;
            // warm gradient
            float rr = 1.0f;
            float gg = 0.35f + 0.35f * t;
            float bb = 0.1f;
            float a = 0.85f * falloff + 0.25f * core;
            put_rgba(img, size, x, y, uc(rr), uc(gg), uc(bb), uc(a));
        }
    }
    return img;
}

std::vector<unsigned char> TextureAtlas::make_tile_smoke(int size) const {
    std::vector<unsigned char> img(size * size * 4, 0);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            float nx = (x + 0.5f) / size * 2.0f - 1.0f;
            float ny = (y + 0.5f) / size * 2.0f - 1.0f;
            float r = std::sqrt(nx * nx + ny * ny);
            float falloff = std::exp(-r * 2.6f);
            float noise = 0.5f + 0.5f * std::sin((nx * 7.3f + ny * 6.1f) * 3.1f);
            float a = 0.8f * falloff * (0.65f + 0.35f * noise);
            float c = 0.55f + 0.25f * noise;
            put_rgba(img, size, x, y, uc(c), uc(c), uc(c), uc(a));
        }
    }
    return img;
}

std::vector<unsigned char> TextureAtlas::make_tile_spark(int size) const {
    std::vector<unsigned char> img(size * size * 4, 0);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            float nx = (x + 0.5f) / size * 2.0f - 1.0f;
            float ny = (y + 0.5f) / size * 2.0f - 1.0f;
            float r = std::sqrt(nx * nx + ny * ny);
            float core = std::exp(-r * 18.0f);
            float a = core;
            put_rgba(img, size, x, y, uc(1.0f), uc(0.95f), uc(0.4f), uc(a));
        }
    }
    return img;
}

std::vector<unsigned char> TextureAtlas::make_tile_rain(int size) const {
    std::vector<unsigned char> img(size * size * 4, 0);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            float nx = (x + 0.5f) / size * 2.0f - 1.0f;
            float ny = (y + 0.5f) / size * 2.0f - 1.0f;

            // thin vertical streak
            float line = std::exp(-std::abs(nx) * 28.0f);

            // stretch the highlight downward
            float body = std::exp(-std::abs(ny + 0.15f) * 1.2f);

            // stronger head of the raindrop
            float head = std::exp(-(nx * nx * 40.0f + (ny - 0.55f) * (ny - 0.55f) * 10.0f));

            float a = 0.80f * line * body + 0.35f * head;
            a = std::min(1.0f, a);

            put_rgba(img, size, x, y, uc(0.72f), uc(0.86f), uc(1.0f), uc(a));
        }
    }

    return img;
}

std::vector<unsigned char> TextureAtlas::make_tile_magic(int size) const {
    std::vector<unsigned char> img(size * size * 4, 0);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            float nx = (x + 0.5f) / size * 2.0f - 1.0f;
            float ny = (y + 0.5f) / size * 2.0f - 1.0f;
            float r = std::sqrt(nx * nx + ny * ny);
            float ring = std::exp(-std::abs(r - 0.45f) * 18.0f);
            float core = std::exp(-r * 10.0f);
            float a = 0.75f * ring + 0.25f * core;
            put_rgba(img, size, x, y, uc(0.65f), uc(0.35f), uc(1.0f), uc(a));
        }
    }
    return img;
}

std::vector<unsigned char> TextureAtlas::make_tile_soft(int size) const {
    // soft round shape with long fade
    // used by embers and snow where we want gentle round dots
    std::vector<unsigned char> img(size * size * 4, 0);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            float nx = (x + 0.5f) / size * 2.0f - 1.0f;
            float ny = (y + 0.5f) / size * 2.0f - 1.0f;
            float r = std::sqrt(nx * nx + ny * ny);
            float a = std::exp(-r * 3.8f);
            put_rgba(img, size, x, y, uc(1.0f), uc(1.0f), uc(1.0f), uc(a));
        }
    }
    return img;
}

void TextureAtlas::build_default_atlas() {
    destroy();

    const int tile = 256;
    const int tiles = m_tile_count;
    const int w = tile * tiles;
    const int h = tile;

    std::vector<unsigned char> atlas(w * h * 4, 0);

    auto blit = [&](const std::vector<unsigned char>& tile_img, int tile_index) {
        for (int y = 0; y < tile; y++) {
            for (int x = 0; x < tile; x++) {
                const int dst_x = tile_index * tile + x;
                const int dst_y = y;
                const int dst = (dst_y * w + dst_x) * 4;
                const int src = (y * tile + x) * 4;
                atlas[dst + 0] = tile_img[src + 0];
                atlas[dst + 1] = tile_img[src + 1];
                atlas[dst + 2] = tile_img[src + 2];
                atlas[dst + 3] = tile_img[src + 3];
            }
        }
    };

    blit(make_tile_fire(tile),  0);
    blit(make_tile_smoke(tile), 1);
    blit(make_tile_spark(tile), 2);
    blit(make_tile_rain(tile),  3);
    blit(make_tile_magic(tile), 4);
    blit(make_tile_soft(tile),  5);

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    m_tile_scale = glm::vec2(1.0f / (float)tiles, 1.0f);
}
