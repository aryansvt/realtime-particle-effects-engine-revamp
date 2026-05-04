#pragma once

#include <glm/glm.hpp>
#include <vector>

class TextureAtlas {
public:
    TextureAtlas() = default;
    ~TextureAtlas();

    void build_default_atlas();
    void destroy();

    unsigned id() const { return m_tex; }
    glm::vec2 tile_uv_scale() const { return m_tile_scale; }
    int tile_count() const { return m_tile_count; }

private:
    unsigned m_tex{0};
    int m_tile_count{6};
    glm::vec2 m_tile_scale{1.0f / 6.0f, 1.0f};

    std::vector<unsigned char> make_tile_fire(int size) const;
    std::vector<unsigned char> make_tile_smoke(int size) const;
    std::vector<unsigned char> make_tile_spark(int size) const;
    std::vector<unsigned char> make_tile_rain(int size) const;
    std::vector<unsigned char> make_tile_magic(int size) const;
    std::vector<unsigned char> make_tile_soft(int size) const;
};
