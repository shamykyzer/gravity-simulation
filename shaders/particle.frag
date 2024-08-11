const char* fragmentSource = "#version 330 core\n"
                             "in vec3 color;\n"
                             "out vec4 FragColor;\n"
                             "void main()\n"
                             "{\n"
                             "    FragColor = vec4(color, 1.0);\n"
                             "}";
