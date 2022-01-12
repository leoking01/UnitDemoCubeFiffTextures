﻿

#pragma execution_character_set("utf-8")
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "shader_m.h"
#include <learnopengl/shader_m.h>

#include <iostream>
#include <string>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


// settings
const unsigned int SCR_WIDTH = 800;// 1920;//1200;// 800;//400;
const unsigned int SCR_HEIGHT = 600;//1080;// 960;//  640;//400;

// camera
glm::vec3 cameraPos = glm::vec3(5.f, 5.f, 5.0f);
glm::vec3 cameraFront = glm::vec3(-5.0f, -5.0f, -5.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaalpha = 0.f;
float deltabeta = 0.f;
float deltatheta = 0.f; //左右转
glm::mat4 currentModel = glm::mat4(1.0f);



bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;



// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    std::cout << "一个可以旋转的正方体（WASD，R）" << std::endl;
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "一个可以旋转的正方体（WASD，R）", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("pos.vs", "color.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    //    for(int i=0;i<sizeof(vertices)/5;i++){
    //        vertices[5*i+0] *=  12.8;
    //        vertices[5*i+1] *=  12.8;
    //        vertices[5*i+2] *=  12.8;
    //        vertices[5*i+3] *=2.8;
    //         vertices[5*i+4] *=2.8;
    //    }

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(1.0f,0.f,0.f),
        glm::vec3(0.f,1.f,0.0f),
        glm::vec3(-1.f,0.f,0.f),
        glm::vec3(0.f,-1.f,0.0f),
        glm::vec3(1.f,1.f,0.f),

        glm::vec3(-1.f,1.f,0.0f),
        glm::vec3(-1.f,-1.f,0.f),
        glm::vec3(1.f,-1.f,0.0f),
        glm::vec3(0.0f,0.0f,1.f),
        glm::vec3(1.0f,0.f,1.f),
        glm::vec3(0.f,1.f,1.0f),

        glm::vec3(-1.f,0.f,1.f),
        glm::vec3(0.f,-1.f,1.0f),
        glm::vec3(1.f,1.f,1.f),
        glm::vec3(-1.f,1.f,1.0f),
        glm::vec3(-1.f,-1.f,1.f),
        glm::vec3(1.f,-1.f,1.0f),

        glm::vec3(0.0f,0.0f,-1.f),
        glm::vec3(1.0f,0.f,-1.f),
        glm::vec3(0.f,1.f,-1.0f),
        glm::vec3(-1.f,0.f,-1.f),
        glm::vec3(0.f,-1.f,-1.0f),
        glm::vec3(1.f,1.f,-1.f),

        glm::vec3(-1.f,1.f,-1.0f),
        glm::vec3(-1.f,-1.f,-1.f),
        glm::vec3(1.f,-1.f,-1.0f)
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6*5*sizeof(float)));
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(33 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2, texture3;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("front.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("left.jpg", &width, &height, &nrChannels, 0);
    //     data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture3
    // --------
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("bottom.jpg", &width, &height, &nrChannels, 0);
    if (data)  {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    unsigned int texture4, texture5, texture6;
    // texture 4
    // ---------
    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    //     int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("back.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else  {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



    // texture  5
    // ---------
    glGenTextures(1, &texture5);
    glBindTexture(GL_TEXTURE_2D, texture5);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    //     int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("right.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else  {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture  6
    // ---------
    glGenTextures(1, &texture6);
    glBindTexture(GL_TEXTURE_2D, texture6);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    //     int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("top.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else  {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    //ourShader.setInt("texture1", 0);
    //ourShader.setInt("texture2", 1);

    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    // -----------------------------------------------------------------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);



        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);
        // calculate the model matrix for each object and pass it to shader before drawing
        currentModel = glm::rotate(currentModel, glm::degrees(deltaalpha), glm::vec3(0.0f, 1.f, 0.f));
        currentModel = glm::rotate(currentModel, glm::degrees(deltabeta), glm::vec3(1.0f, 0.f, 0.f));
        currentModel = glm::rotate(currentModel, glm::degrees(deltatheta), glm::vec3(0.0f, 0.f, 1.f));

        for (int i = 0; i < 1; i++) {
            glm::mat4 thismodel = glm::translate(currentModel, cubePositions[ i ]);
            ourShader.setMat4("model", thismodel);

            // bind textures on corresponding texture units
            //glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            //            glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
            glDrawArrays(GL_TRIANGLES, 0, 2 * 3);


            //glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);        //激活了哪个，下面的绑定就会是哪个纹理，这个参数很关键。
            glBindTexture(GL_TEXTURE_2D, texture4);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * 5 * sizeof(float)));
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(33 * sizeof(float)));
            glEnableVertexAttribArray(1);
            //            glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
            glDrawArrays(GL_TRIANGLES, 0, 2 * 3);


            //glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);        //激活了哪个，下面的绑定就会是哪个纹理，这个参数很关键。
            glBindTexture(GL_TEXTURE_2D, texture2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(12 * 5 * sizeof(float)));
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(63 * sizeof(float)));
            glEnableVertexAttribArray(1);
            //            glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
            glDrawArrays(GL_TRIANGLES, 0, 2 * 3);


            //glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);        //激活了哪个，下面的绑定就会是哪个纹理，这个参数很关键。
            glBindTexture(GL_TEXTURE_2D, texture5);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(18 * 5 * sizeof(float)));
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(93 * sizeof(float)));
            glEnableVertexAttribArray(1);
            //            glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
            glDrawArrays(GL_TRIANGLES, 0, 2 * 3);

            //glBindVertexArray(VAO);    //就这么一个vao，不绑也行
            glActiveTexture(GL_TEXTURE0);        //激活了哪个，下面的绑定就会是哪个纹理，这个参数很关键。
            glBindTexture(GL_TEXTURE_2D, texture3);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * 12 * 5 * sizeof(float)));
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(123 * sizeof(float)));
            glEnableVertexAttribArray(1);
            //            glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
            glDrawArrays(GL_TRIANGLES, 0, 2 * 3);


            //glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);        //激活了哪个，下面的绑定就会是哪个纹理，这个参数很关键。
            glBindTexture(GL_TEXTURE_2D, texture6 );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(30 * 5 * sizeof(float)));
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(153 * sizeof(float)));
            glEnableVertexAttribArray(1);
            //            glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
            glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

////// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
////// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//    deltaalpha = 0;
//    deltabeta = 0;
//    deltatheta = 0;
//    float cameraSpeed = 2.5 * deltaTime ;//*1.9;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//    {
//        deltabeta -= 0.00001f;
//    }
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//    {
//        deltabeta += 0.00001f;
//    }
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//    {
//        deltaalpha -= 0.00001f;
//    }
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//    {
//        deltaalpha += 0.00001f;
//    }
//    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
//    {
//        deltatheta -= 0.00001f;
//    }
//    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
//    {
//        deltatheta += 0.00001f;
//    }
//    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
//    {
//        currentModel = glm::mat4(1.0f);
//    }
//}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}




// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f    ; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
