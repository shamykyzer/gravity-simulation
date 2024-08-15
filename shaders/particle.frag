const char* fragmentSource = "#version 330 core\n"
                             "in vec3 color;\n"
                             "out vec4 FragColor;\n"
                             "void main()\n"
                             "{\n"
                             "    float intensity = length(gl_FragCoord.xy / vec2(800, 600));\n"
                             "    FragColor = vec4(color * intensity, 1.0);\n"
                             "}";
