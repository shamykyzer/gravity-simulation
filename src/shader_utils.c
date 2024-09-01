#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path) {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    FILE * VertexShaderFile = fopen(vertex_file_path, "r");
    if (VertexShaderFile == NULL) {
        fprintf(stderr, "Failed to open vertex shader file: %s\n", vertex_file_path);
        return 0;
    }
    fseek(VertexShaderFile, 0, SEEK_END);
    long VertexShaderFileSize = ftell(VertexShaderFile);
    rewind(VertexShaderFile);
    char * VertexShaderCode = (char *)malloc(VertexShaderFileSize + 1);
    fread(VertexShaderCode, 1, VertexShaderFileSize, VertexShaderFile);
    VertexShaderCode[VertexShaderFileSize] = '\0';
    fclose(VertexShaderFile);

    // Read the Fragment Shader code from the file
    FILE * FragmentShaderFile = fopen(fragment_file_path, "r");
    if (FragmentShaderFile == NULL) {
        fprintf(stderr, "Failed to open fragment shader file: %s\n", fragment_file_path);
        free(VertexShaderCode);
        return 0;
    }
    fseek(FragmentShaderFile, 0, SEEK_END);
    long FragmentShaderFileSize = ftell(FragmentShaderFile);
    rewind(FragmentShaderFile);
    char * FragmentShaderCode = (char *)malloc(FragmentShaderFileSize + 1);
    fread(FragmentShaderCode, 1, FragmentShaderFileSize, FragmentShaderFile);
    FragmentShaderCode[FragmentShaderFileSize] = '\0';
    fclose(FragmentShaderFile);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    glShaderSource(VertexShaderID, 1, (const char **)&VertexShaderCode , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        char * VertexShaderErrorMessage = (char *)malloc(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, VertexShaderErrorMessage);
        fprintf(stderr, "%s\n", VertexShaderErrorMessage);
        free(VertexShaderErrorMessage);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    glShaderSource(FragmentShaderID, 1, (const char **)&FragmentShaderCode , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        char * FragmentShaderErrorMessage = (char *)malloc(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, FragmentShaderErrorMessage);
        fprintf(stderr, "%s\n", FragmentShaderErrorMessage);
        free(FragmentShaderErrorMessage);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        char * ProgramErrorMessage = (char *)malloc(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage);
        fprintf(stderr, "%s\n", ProgramErrorMessage);
        free(ProgramErrorMessage);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    free(VertexShaderCode);
    free(FragmentShaderCode);

    return ProgramID;
}
