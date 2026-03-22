#include "shaders.hpp"
#include <fstream>
#include <format>
#include <SDL3/SDL.h>

using std::string;

static string getShaderLog(GLuint shader) {
    if (glIsShader(shader)) {
        std::string message = "";

        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0) {
            message = infoLog;
        }

        delete[] infoLog;

        return message;
    } else {
        return "Wrong shader handle";
    }
}

static string getProgramLog(GLuint shaderProgram) {
    if (glIsProgram(shaderProgram)) {
        std::string message = "";

        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(shaderProgram, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0) {
            message = infoLog;
        }

        delete[] infoLog;
        return message;
    } else {
        return "Wrong shader program handle";
    }
}

static string loadShaderFromFile(const char* filename) {
    std::ifstream infile(filename);
    if (!infile) {
        throw std::runtime_error(std::format("Cannot load shader file \"{}\"", filename));
    }
    string shader_text((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    return shader_text;
}

static GLuint compileShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const GLchar* lines[] = {
        source };
    glShaderSource(shader, 1, lines, NULL);
    glCompileShader(shader);
    GLint isCompiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled != GL_TRUE) {
        std::string msg;
        if (shaderType == GL_VERTEX_SHADER) {
            msg = "Vertex shader compile error: ";
        } else if (shaderType == GL_FRAGMENT_SHADER) {
            msg = "Fragment shader compile error: ";
        } else {
            msg = "Shader compile error: ";
        }
        throw std::runtime_error(msg + getShaderLog(shader));
    }
    return shader;
}

static GLuint shaderFromFile(const char* filename, GLenum shaderType) {
    string text = loadShaderFromFile(filename);
    return compileShader(text.c_str(), shaderType);
}

static GLuint compileShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        throw std::runtime_error("Shader link error: " + getProgramLog(program));
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
};

void ShaderProgram::use() {
    glUseProgram(program);
}

ShaderProgram::~ShaderProgram() {
    if (glIsProgram(program)) {
        glDeleteProgram(program);
    }
}

void ShaderProgram::loadFromString(const char* vertexShaderCode, const char* fragmentShaderCode) {
    GLuint vertexShader = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);
    program = compileShaderProgram(vertexShader, fragmentShader);
}

void ShaderProgram::loadFromFiles(const char* vertexShaderFile, const char* fragmentShaderFile) {
    string vsCode = loadShaderFromFile(vertexShaderFile);
    string fsCode = loadShaderFromFile(fragmentShaderFile);
    loadFromString(vsCode.c_str(), fsCode.c_str());
}

void BasicShader::init() {
    loadFromFiles("assets/basic.vert", "assets/basic.frag");

    glUseProgram(program);

    GLint tex_location = glGetUniformLocation(program, "tex");
    if (tex_location < 0) throw std::runtime_error("Can't find \"tex\" uniform in shaders");
    glUniform1i(tex_location, 0);

    GLint tbo_location = glGetUniformLocation(program, "spriteData");
    if (tbo_location < 0) throw std::runtime_error("Can't find \"spriteData\" uniform in shaders");
    glUniform1i(tbo_location, 1);

    GLint scale_location = glGetUniformLocation(program, "scale");
    if (scale_location < 0) throw std::runtime_error("Can't find \"scale\" uniform in shaders");
    glUniform4f(scale_location, 0.0, 0.0, 1.0, 1.0);
}
