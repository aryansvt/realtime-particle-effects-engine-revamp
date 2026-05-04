#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

static void check_shader(unsigned shader, const char* name) {
    int ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        int len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string log;
        log.resize(len > 0 ? (size_t)len : 1);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << "shader compile failed " << name << "\n" << log << "\n";
    }
}

static void check_program(unsigned prog) {
    int ok = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        int len = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        std::string log;
        log.resize(len > 0 ? (size_t)len : 1);
        glGetProgramInfoLog(prog, len, nullptr, log.data());
        std::cerr << "program link failed\n" << log << "\n";
    }
}

Shader::Shader(const std::string& vs_path, const std::string& fs_path) {
    load(vs_path, fs_path);
}

std::string Shader::read_file(const std::string& path) {
    std::ifstream f(path);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

unsigned Shader::compile(unsigned type, const std::string& src, const char* debug_name) {
    unsigned s = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(s, 1, &c, nullptr);
    glCompileShader(s);
    check_shader(s, debug_name);
    return s;
}

void Shader::load(const std::string& vs_path, const std::string& fs_path) {
    destroy();

    const std::string vs = read_file(vs_path);
    const std::string fs = read_file(fs_path);

    unsigned vs_id = compile(GL_VERTEX_SHADER, vs, vs_path.c_str());
    unsigned fs_id = compile(GL_FRAGMENT_SHADER, fs, fs_path.c_str());

    m_program = glCreateProgram();
    glAttachShader(m_program, vs_id);
    glAttachShader(m_program, fs_id);
    glLinkProgram(m_program);
    check_program(m_program);

    glDeleteShader(vs_id);
    glDeleteShader(fs_id);
}

void Shader::destroy() {
    if (m_program) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void Shader::use() const {
    glUseProgram(m_program);
}

void Shader::set_mat4(const char* name, const glm::mat4& m) const {
    const int loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::set_vec3(const char* name, const glm::vec3& v) const {
    const int loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform3fv(loc, 1, &v[0]);
}

void Shader::set_vec4(const char* name, const glm::vec4& v) const {
    const int loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform4fv(loc, 1, &v[0]);
}

void Shader::set_vec2(const char* name, const glm::vec2& v) const {
    const int loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform2fv(loc, 1, &v[0]);
}

void Shader::set_float(const char* name, float v) const {
    const int loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform1f(loc, v);
}

void Shader::set_int(const char* name, int v) const {
    const int loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform1i(loc, v);
}
