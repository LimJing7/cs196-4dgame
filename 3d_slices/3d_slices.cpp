#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include "shader.h"
#include "comvec3.h"
#include "lin_interpolation.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//GLOBALS
// Window dimensions
const GLuint WIDTH = 600, HEIGHT = 600;

// framerate
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


int main(){

    // Data
    GLfloat vertices[] = {
        // Positions           
         0.5f,   0.5f,   0.5f,   0.5f,
         0.5f,  -0.5f,   0.5f,   0.5f,  
        -0.5f,  -0.5f,   0.5f,   0.5f,
        -0.5f,   0.5f,   0.5f,   0.5f,
         0.5f,   0.5f,  -0.5f,   0.5f,
         0.5f,  -0.5f,  -0.5f,   0.5f,
        -0.5f,  -0.5f,  -0.5f,   0.5f,
        -0.5f,   0.5f,  -0.5f,   0.5f,
         0.5f,   0.5f,   0.5f,  -0.5f,
         0.5f,  -0.5f,   0.5f,  -0.5f,  
        -0.5f,  -0.5f,   0.5f,  -0.5f,
        -0.5f,   0.5f,   0.5f,  -0.5f,
         0.5f,   0.5f,  -0.5f,  -0.5f,
         0.5f,  -0.5f,  -0.5f,  -0.5f,
        -0.5f,  -0.5f,  -0.5f,  -0.5f,
        -0.5f,   0.5f,  -0.5f,  -0.5f
    };

    /* vertices order
    ftr, fbr, fbl,
    ftr, fbl, ftl,
    btr, ftr, ftl,
    btr, ftl, btl,
    btr, bbr, fbr,
    btr, fbr, ftr,
    ftl, fbl, bbl,
    ftl, bbl, btl,
    fbr, bbr, bbl,
    fbr, bbl, fbl,
    btl, bbl, bbr,
    btl, bbr, btr,
    */

    GLuint indices[] = {
        // Near Cube
         0,  1,  2,
         0,  2,  3,
         4,  0,  3,
         4,  3,  7,
         4,  5,  1,
         4,  1,  0,
         3,  2,  6,
         3,  6,  7,
         1,  5,  6,
         1,  6,  2,
         7,  6,  5,
         7,  5,  4,

        // Far Cube
         8,  9, 10,
         8, 10, 11,
        12,  8, 11,
        12, 11, 15,
        12, 13,  9,
        12,  9,  8,
        11, 10, 14,
        11, 14, 15,
         9, 13, 14,
         9, 14, 10,
        15, 14, 13,
        15, 13, 12,

        // Front Cube
         0,  1,  2,
         0,  2,  3,
         8,  0,  3,
         8,  3, 11,
         8,  9,  1,
         8,  1,  0,
         3,  2, 10,
         3, 10, 11,
         1,  9, 10,
         1, 10,  2,
        11, 10,  9,
        11,  9,  8,

        //Back Cube
        12, 13, 14,
        12, 14, 15,
         4, 12, 15,
         4, 15,  7,
         4,  5, 13,
         4, 13, 12,
        15, 14,  6,
        15,  6,  7,
        13,  5,  6,
        13,  6, 14,
         7,  6,  5,
         7,  5,  4,

        // Top Cube
         0,  8, 11,
         0, 11,  3,
         4,  0,  3,
         4,  3,  7,
         4, 12,  8,
         4,  8,  0,
         3, 11, 15,
         3, 15,  7,
         8, 12, 15,
         8, 15, 11,
         7, 15, 12,
         7, 12,  4,

        // Bottom Cube
         9,  1,  2,
         9,  2, 10,
        13,  9, 10,
        13, 10, 14,
        13,  5,  1,
        13,  1,  9,
        10,  2,  6,
        10,  6, 14,
         1,  5,  6,
         1,  6,  2,
        14,  6,  5,
        14,  5, 13,

        // Left Cube
        11, 10,  2,
        11,  2,  3,
        15, 11,  3,
        15,  3,  7,
        15, 14, 10,
        15, 10, 11,
         3,  2,  6,
         3,  6,  7,
        10, 14,  6,
        10,  6,  2,
         7,  6, 14,
         7, 14, 15,

        // Right Cube
         0,  1,  9,
         0,  9,  8,
         4,  0,  8,
         4,  8, 12,
         4,  5,  1,
         4,  1,  0,
         8,  9, 13,
         8, 13, 12,
         1,  5, 13,
         1, 13,  9,
        12, 13,  5,
        12,  5,  4
    };
    
    GLuint lines[] = {
         0,  1,
         1,  2,
         2,  3,
         3,  0,
         3,  7,
         7,  4,
         4,  0,
         4,  5,
         5,  1,
         5,  6,
         6,  2,
         7,  6,
         8, 11,
        11, 15,
        15, 12,
        12,  8,
         8,  9,
         9, 10,
        10, 11,
        12, 13,
        13,  9,
        10, 14,
        14, 13,
        14, 15,
         0,  8,
         1,  9,
         2, 10,
         3, 11,
         4, 12,
         5, 13,
         6, 14,
         7, 15
    };
    
    //setup window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D Slice", nullptr, nullptr);
    int Width, Height;
    glfwGetWindowSize(window, &Width, &Height);
    glfwMakeContextCurrent(window);

    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

 
    glViewport(0, 0, Width, Height);    

    glfwSetKeyCallback(window, key_callback);

    Shader ourShader("shaders/normal.vert", "shaders/normal.frag");

    /*glm::vec4 start = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 end   = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 test  = lin_interpolation(start, end, 0.5);
    std::cout<<test.x<<test.y<<test.z<<std::endl;*/

    //std::set<comVec3> vertices_to_draw;
    //std::set<comVec3>::iterator it;
    
    /*for (int i=0; i<64; i+=2){
        glm::vec4 startvec = glm::vec4(vertices[lines[i]*4], vertices[lines[i]*4+1], vertices[lines[i]*4+2], vertices[lines[i]*4+3]);
        glm::vec4 endvec = glm::vec4(vertices[lines[i+1]*4], vertices[lines[i+1]*4+1], vertices[lines[i+1]*4+2], vertices[lines[i+1]*4+3]);
        float w=0.0;
        std::vector<comVec3> points = lin_interpolation_l2p(startvec, endvec, w);
        vertices_to_draw.insert(points.begin(), points.end());
    }

    std::cout<<"Vertices:"<<std::endl;
    for (it=vertices_to_draw.begin(); it!=vertices_to_draw.end(); it++){
        std::cout<<glm::to_string((*it).content)<<std::endl;
    }*/

    /*for (int i=64; i<96; i+=3){
        float w=0.0;
        glm::vec4 a = glm::vec4(vertices[indices[i]*4], vertices[indices[i]*4+1], vertices[indices[i]*4+2], vertices[indices[i]*4+3]);
        glm::vec4 b = glm::vec4(vertices[indices[i+1]*4], vertices[indices[i+1]*4+1], vertices[indices[i+1]*4+2], vertices[indices[i+1]*4+3]);
        glm::vec4 c = glm::vec4(vertices[indices[i+2]*4], vertices[indices[i+2]*4+1], vertices[indices[i+2]*4+2], vertices[indices[i+2]*4+3]);
        std::vector<comVec3> points = lin_interpolation_t2l(a,b,c,w);
        std::vector<comVec3>::iterator pit;
        //std::cout<<vertices[indices[i]*4]<<std::endl;
        //std::cout<<a.x<<std::endl;
        
        std::cout<<glm::to_string(a)<<std::endl;
        std::cout<<glm::to_string(b)<<std::endl;
        std::cout<<glm::to_string(c)<<std::endl;
        for(pit=points.begin(); pit!=points.end(); pit++){
            std::cout<<glm::to_string((*pit).content)<<std::endl;
        }
        std::cout<<"\n";
    }*/
    
    std::vector<comVec3> list_ver;
    std::vector<comVec3>::iterator itv=list_ver.begin();
    float w = 0.0;
    
    for (int i=0; i<288; i+=36){
        std::vector<comVec3> trigs = lin_interpolation_c2t(indices, vertices, i, 12, w);
        //std::cout<<trigs.size()<<std::endl;
        itv=list_ver.insert(itv, trigs.begin(), trigs.end());
    }

    
    std::cout<<"Vertices:"<<std::endl;
    for (itv=list_ver.begin(); itv!=list_ver.end(); itv++){
        std::cout<<glm::to_string((*itv).content)<<std::endl;
    }
    //std::cout<<list_ver.size()<<std::endl;
    
    std::vector<GLfloat> vertices_to_draw;
    for (itv=list_ver.begin(); itv!=list_ver.end(); itv++){
        vertices_to_draw.push_back((*itv).content.x);
        vertices_to_draw.push_back((*itv).content.y);
        vertices_to_draw.push_back((*itv).content.z);
    }


    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_to_draw.size(), &vertices_to_draw[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);  
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //main loop
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame; 

        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ourShader.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POLYGON, 0, 8); 
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    /*glDeleteBuffers(1, &EBO);    */
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE); 
}


