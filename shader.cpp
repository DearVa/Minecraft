#include "shader.h"

Shader::Shader(const string &vsFile, const string &fsFile) {
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    try {
        // 打开文件
        vShaderFile.open(vsFile);
        fShaderFile.open(fsFile);
        stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        // 2. 编译着色器
        GLuint vertex, fragment;
        // 顶点着色器    vs
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, false);
        // 片段着色器    fs
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, false);
        // 着色器程序
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        CheckCompileErrors(program, false);
        // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了    
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } catch (exception e) {
        cout << "shader出现错误" << endl << e.what() << endl;
    }
}

void Shader::Use() {
    glUseProgram(program);
}

void Shader::SetBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::SetInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::SetFloat(const string &name, float value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

Shader::~Shader() {
    if (program) {
        glDeleteProgram(program);
    }
}

void Shader::CheckCompileErrors(GLuint shader, bool isProgram) {
    int success;
    char infoLog[1024];
    if (isProgram) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            throw exception(infoLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw exception(infoLog);
        }
    }
}