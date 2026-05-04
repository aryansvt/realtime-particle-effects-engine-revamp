#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader() = default;
    Shader(const std::string& vs_path, const std::string& fs_path);

    void load(const std::string& vs_path, const std::string& fs_path);
    void destroy();

    void use() const;
    unsigned id() const { return m_program; }

    void set_mat4(const char* name, const glm::mat4& m) const;
    void set_vec3(const char* name, const glm::vec3& v) const;
    void set_vec4(const char* name, const glm::vec4& v) const;
    void set_vec2(const char* name, const glm::vec2& v) const;
    void set_float(const char* name, float v) const;
    void set_int(const char* name, int v) const;

private:
    unsigned m_program{0};

    static std::string read_file(const std::string& path);
    static unsigned compile(unsigned type, const std::string& src, const char* debug_name);
};
