#include "shader.h"

#include <iostream>
#include <cmath>

// GLEW
// A little bit of hacking here to get my autocomplete to work
#ifdef CLANG_COMPLETE_ONLY
    #define GL_GLEXT_PROTOTYPES
#ifdef __LINUXOS__
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
#ifdef __APPLEOS__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#endif
#else
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif

// GLFW
#include <GLFW/glfw3.h>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the 
// game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // OS X requests 3.3 differently.
#ifdef __APPLEOS__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", 
                                          nullptr, nullptr);    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving
    // function pointers and extensions
    // More hacking for my autocomplete.
#ifndef CLANG_COMPLETE_ONLY
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    
#endif

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // ---------- BEGIN OPENGL ----------- //
    
    // Shader creation
    Shader simpleShader("vertex_simple.glsl", "fragment_uniforms.glsl");

    // Data
    GLfloat vertices[] = {
         0.5f,   0.5f,   0.0f,
         0.5f,  -0.5f,   0.0f,
        -0.5f,  -0.5f,   0.0f,
        -0.5f,   0.5f,   0.0f
    };

    GLuint indices[] = {
        0,  1,  3
        //1,  2,  3
    };

    // Set up buffer stuff
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Associate VBO and EBO with VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3*sizeof(GL_FLOAT), 
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // We're done, unbind

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved
        // etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw, update uniform color
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue)/2) + 0.5f;
        GLint vertexColorLocation = glGetUniformLocation(simpleShader.Program, "ourColor");
        simpleShader.Use();
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        // Wireframe mode
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
