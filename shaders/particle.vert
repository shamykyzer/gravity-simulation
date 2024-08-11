const char* vertexSource = "#version 330 core\n"
                           "layout(location = 0) in vec2 aPos;\n"
                           "layout(location = 1) in vec3 aColor;\n"
                           "out vec3 color;\n"
                           "void main()\n"
                           "{\n"
                           "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
                           "    color = aColor;\n"
                           "}";
