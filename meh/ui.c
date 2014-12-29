#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char *vertex_src = ""
        "#version 330 core\n"
        "in vec3 aPos;\n"
        "\n"
        "void main(){\n"
        "   gl_Position = 0.5*vec4(aPos.xyz, 1);\n"
        "}";

const char *fragment_src = ""
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main(){\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}";


GLFWwindow* init_graphic_apis(){
    
    if (!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create context GLFW\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
//    glewExperimental = GL_TRUE;
//    GLenum st = glewInit();
//    if (st != GLEW_OK) {
//        fprintf(stderr, "Failed to initialize GLEW %s\n", glewGetErrorString(st));
//        return NULL;
//    }

    return window;
}


GLuint loadShaders(const char * vertex, const char * fragment){

    GLuint vshId = glCreateShader(GL_VERTEX_SHADER);
    GLuint frshId = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    int logLen;

    // Compile
    glShaderSource(vshId, 1, &vertex , NULL);
    glCompileShader(vshId);

    glShaderSource(frshId, 1, &fragment , NULL);
    glCompileShader(frshId);

    // Check
    glGetShaderiv(vshId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vshId, GL_INFO_LOG_LENGTH, &logLen);

    char *msg = malloc(logLen);
    glGetShaderInfoLog(vshId, logLen, NULL, msg);
    printf("%s\n", msg);

    glGetShaderiv(frshId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(frshId, GL_INFO_LOG_LENGTH, &logLen);

    msg = realloc(msg, logLen);
    glGetShaderInfoLog(frshId, logLen, NULL, msg);
    printf("%s\n", msg);

    // Link the program
    GLuint progId = glCreateProgram();
    glAttachShader(progId, vshId);
    glAttachShader(progId, frshId);
    glLinkProgram(progId);

    // Check the program
    glGetProgramiv(progId, GL_LINK_STATUS, &result);
    glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &logLen);

    msg = realloc(msg, logLen);
    glGetProgramInfoLog(progId, logLen, NULL, msg);
    printf("%s\n", msg);

    glDeleteShader(vshId);
    glDeleteShader(frshId);
    free(msg);
    return progId;
}

void setup_program1(){
    // create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create VBO's
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    // bind vbo to program attr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);

}

int main(void) {
    GLFWwindow* window = init_graphic_apis();
    if (window == NULL){
        return -1;
    }

    loadShaders(vertex_src, fragment_src);
    setup_program1();

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        float ratio = width / (float) height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}