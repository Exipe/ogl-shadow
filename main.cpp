
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "cube.h"
#include "model.h"
#include "Camera.h"
#include "depth_map.h"
#include "quad.h"

#define SHADOW_MAP_WIDTH 1024
#define SHADOW_MAP_HEIGHT 1024

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

ShadowProgram initShadowProgram() {
    auto vertex = compileShader("shader/shadow-vertex.glsl", GL_VERTEX_SHADER);
    auto fragment = compileShader("shader/shadow-fragment.glsl", GL_FRAGMENT_SHADER);

    auto program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    return ShadowProgram(program);
}

Program initDepthDemoProgram() {
    auto vertex = compileShader("shader/depth-demo-vertex.glsl", GL_VERTEX_SHADER);
    auto fragment = compileShader("shader/depth-demo-fragment.glsl", GL_FRAGMENT_SHADER);

    auto program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    return Program(program);
}

DepthProgram initDepthProgram() {
    auto vertex = compileShader("shader/depth-vertex.glsl", GL_VERTEX_SHADER);
    auto fragment = compileShader("shader/depth-fragment.glsl", GL_FRAGMENT_SHADER);

    auto program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    return DepthProgram(program);
}

StandardProgram initStandardProgram() {
    auto vertex = compileShader("shader/vertex.glsl", GL_VERTEX_SHADER);
    auto fragment = compileShader("shader/fragment.glsl", GL_FRAGMENT_SHADER);

    auto program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    return StandardProgram(program);
}

Camera cam(glm::vec3(0.0, 0.0, 5.0), -90, 0);

float lastX = (float) WINDOW_WIDTH / 2;
float lastY = (float) WINDOW_HEIGHT / 2;

void mouseMove(GLFWwindow* window, double posX, double posY) {
    float offsetX = (posX - lastX) * 0.1;
    float offsetY = (lastY - posY) * 0.1;
    lastX = posX;
    lastY = posY;

    cam.rotate(offsetX, offsetY);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(800, 600, "OGL Shadow", nullptr, nullptr);
    if(window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseMove);

    //initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    auto depthDemoProgram = initDepthDemoProgram();



    auto program = initStandardProgram();
    program.use();

    Light light {
            glm::vec3(0, 20, 20),
            glm::vec3(0.2, 0.2, 0.2),
            glm::vec3(0.5, 0.5, 0.5),
            glm::vec3(1.0, 1.0, 1.0)
    };
    program.setLight(light);

    auto projection = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);
    program.setProjection(projection);



    auto depthProgram = initDepthProgram();
    depthProgram.use();

    auto depthMap = initDepthMap(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);

    auto lightView = glm::lookAt(light.position, //render scene from light's point of view (for depth map)
                                 glm::vec3(0.0f, 0.0f, -40.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);

    auto lightSpace = lightProjection * lightView;
    depthProgram.setLightSpace(lightSpace);



    auto shadowProgram = initShadowProgram();
    shadowProgram.use();

    shadowProgram.setLight(light);
    shadowProgram.setProjection(projection);
    shadowProgram.setLightSpace(lightSpace);
    shadowProgram.setDepthMap(1);



    auto dinoModel = glm::mat4(1.0) ;
    dinoModel = glm::translate(dinoModel, glm::vec3(0.5, -0.5, -40.0));
    dinoModel = glm::scale(dinoModel, glm::vec3(0.3, 0.3, 0.3));

    Model dino;
    if(!loadModel(dino, "model", "model/12221_Cat_v1_l3.obj")){
        return -1;
    }

    auto garfModel = glm::mat4(1.0) ;
    garfModel = glm::translate(garfModel, glm::vec3(0.0, -3.5, -20.0));
    garfModel = glm::rotate(garfModel, glm::radians(-90.0f) ,glm::vec3(1.0, 0.0, 0.0));
    garfModel = glm::scale(garfModel, glm::vec3(0.5, 0.5, 0.5));

    Model garf;
    if(!loadModel(garf, "model", "model/20430_Cat_v1_NEW.obj")){
        return -1;
    }

    auto duckLModel = glm::mat4(1.0) ;
    duckLModel = glm::translate(duckLModel, glm::vec3(6.0, -8.5, -20.0));
    duckLModel = glm::rotate(duckLModel, glm::radians(-90.0f) ,glm::vec3(1.0, 0.0, 1.0));
    duckLModel = glm::scale(duckLModel, glm::vec3(0.5, 0.5, 0.5));

    Model duckL;
    if(!loadModel(duckL, "model", "model/12248_Bird_v1_l2.obj")){
        return -1;
    }

    auto duckRModel = glm::mat4(1.0) ;
    duckRModel = glm::translate(duckRModel, glm::vec3(-6.0, -8.5, -20.0));
    duckRModel = glm::rotate(duckRModel, glm::radians(-90.0f) ,glm::vec3(1.0, 0.0, -1.0));
    duckRModel = glm::scale(duckRModel, glm::vec3(0.5, 0.5, 0.5));

    Model duckR;
    if(!loadModel(duckR, "model", "model/12248_Bird_v1_l2.obj")){
        return -1;
    }

    auto eyeRModel = glm::mat4(1.0) ;
    eyeRModel = glm::translate(eyeRModel, glm::vec3(-5.0, 7.0, -30.0));
    eyeRModel = glm::rotate(eyeRModel, glm::radians(-90.0f) ,glm::vec3(1.0, 0.0, 0.0));
    eyeRModel = glm::scale(eyeRModel, glm::vec3(0.1, 0.1, 0.1));

    Model eyeR;
    if(!loadModel(eyeR, "model", "model/12221_Cat_v1_l3.obj")){
        return -1;
    }

    auto eyeLModel = glm::mat4(1.0) ;
    eyeLModel = glm::translate(eyeLModel, glm::vec3(5.0, 7.0, -30.0));
    eyeLModel = glm::rotate(eyeLModel, glm::radians(-90.0f) ,glm::vec3(1.0, 0.0, 0.0));
    eyeLModel = glm::scale(eyeLModel, glm::vec3(0.1, 0.1, 0.1));

    Model eyeL;
    if(!loadModel(eyeL, "model", "model/12221_Cat_v1_l3.obj")){
        return -1;
    }

    auto skeleModel = glm::mat4(1.0);
    skeleModel = glm::translate(skeleModel, glm::vec3(0.0, -10.5, -50.0));
    skeleModel = glm::rotate(skeleModel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    skeleModel = glm::scale(skeleModel, glm::vec3(0.65, 0.65, 0.65));

    Model skull;
    if(!loadModel(skull, "model", "model/12140_Skull_v3_L2.obj")) {
        return -1;
    }

    float bob = 0;
    float bol = 1;

    auto cubeModel = glm::mat4(1.0);
    cubeModel = glm::translate(cubeModel, glm::vec3(0, 0.0, -50));
    cubeModel = glm::scale(cubeModel, glm::vec3(20, 15, 10));

    auto cube = initCube();

    auto quad = initQuad();

    auto mode = 1;

    while(!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_W)) {
            cam.moveForward(0.1);
        }
        if(glfwGetKey(window, GLFW_KEY_S)) {
            cam.moveBackward(0.1);
        }
        if(glfwGetKey(window, GLFW_KEY_D)) {
            cam.moveRight(0.1);
        }
        if(glfwGetKey(window, GLFW_KEY_A)) {
            cam.moveLeft(0.1);
        }
        if(glfwGetKey(window, GLFW_KEY_1)) {
            mode = 1;
        }
        if(glfwGetKey(window, GLFW_KEY_2)) {
            mode = 2;
        }
        if(glfwGetKey(window, GLFW_KEY_3)) {
            mode = 3;
        }

        if(bob == 5.0)
            bol = -1.0;


        else if(bob == 0.0 )
            bol = 1.0;


        bob =bob + bol;


        eyeLModel = glm:: translate(eyeLModel, glm::vec3(0.0, 0.0, bol));
        eyeRModel = glm::translate(eyeRModel, glm::vec3(0.0, 0.0, bol));
        dinoModel = glm::rotate(dinoModel, (float)glfwGetTime()/1000 ,glm::vec3(0.0, 0.0, 1.0));
        if(mode == 1) {
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glClearColor(0.1, 0, 0.1, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




            program.use();

            program.setCamera(cam.cameraPos);
            program.setView(cam.getViewMatrix());

            program.setModel(skeleModel);
            skull.render(&program);

            program.setModel(garfModel);
            garf.render(&program);

            program.setModel(eyeLModel);
            eyeL.render(&program);


            program.setModel(eyeRModel);
            eyeR.render(&program);

            program.setModel(duckLModel);
            duckL.render(&program);

            program.setModel(duckRModel);
            duckR.render(&program);

            program.setModel(dinoModel);
            dino.render(&program);

            program.setModel(cubeModel);
            cube.render(&program);
        }

        if(mode == 2 || mode == 3) {
            depthProgram.use();
            glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
            depthMap.bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            depthProgram.setModel(skeleModel);
            skull.render();

            depthProgram.setModel(garfModel);
            garf.render();

            depthProgram.setModel(eyeLModel);
            eyeL.render();

            depthProgram.setModel(eyeRModel);
            eyeR.render();

            depthProgram.setModel(duckLModel);
            duckL.render();

            depthProgram.setModel(duckRModel);
            duckR.render();

            depthProgram.setModel(dinoModel);
            dino.render();

            /*depthProgram.setModel(cubeModel);
            cube.render();*/

            depthMap.unbind();
        }

        if(mode == 2) {
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glClearColor(0.1, 0, 0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shadowProgram.use();
            depthMap.bindTexture(1);

            shadowProgram.setCamera(cam.cameraPos);
            shadowProgram.setView(cam.getViewMatrix());

            shadowProgram.setModel(skeleModel);
            skull.render(&shadowProgram);

            shadowProgram.setModel(garfModel);
            garf.render(&shadowProgram);

            shadowProgram.setModel(eyeLModel);
            eyeL.render(&shadowProgram);

            shadowProgram.setModel(eyeRModel);
            eyeR.render(&shadowProgram);

            shadowProgram.setModel(duckLModel);
            duckL.render(&shadowProgram);

            shadowProgram.setModel(duckRModel);
            duckR.render(&shadowProgram);

            shadowProgram.setModel((dinoModel));
            dino.render(&shadowProgram);

            /*shadowProgram.setModel(cubeModel);
            cube.render(&shadowProgram);*/
        }

        if(mode == 3) {
            depthDemoProgram.use();
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            depthMap.bindTexture();
            quad.render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
