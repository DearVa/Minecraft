#include "shader.h"

Shader::Shader(const string &vsFile, const string &fsFile) {
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    try {
        // ���ļ�
        vShaderFile.open(vsFile);
        fShaderFile.open(fsFile);
        stringstream vShaderStream, fShaderStream;
        // ��ȡ�ļ��Ļ������ݵ���������
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // �ر��ļ�������
        vShaderFile.close();
        fShaderFile.close();
        // ת����������string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        // 2. ������ɫ��
        GLuint vertex, fragment;
        // ������ɫ��    vs
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, false);
        // Ƭ����ɫ��    fs
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, false);
        // ��ɫ������
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        CheckCompileErrors(program, false);
        // ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��    
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } catch (exception e) {
        cout << "shader���ִ���" << endl << e.what() << endl;
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