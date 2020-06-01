
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

Model eyeR, eyeL, dino, garf, duckL, duckR, skull;

int prepareModels(std::vector<Model*> &models) {
    if(!loadModel(dino, "model", "model/12221_Cat_v1_l3.obj")) return -1;
    dino.rotationAxis = glm::vec3(0.0, 0.0, 1.0);
    dino.position = glm::vec3(0.5, -0.5, -40.0);
    dino.scale = 0.3;
    models.push_back(&dino);

    if(!loadModel(garf, "model", "model/20430_Cat_v1_NEW.obj")) return -1;
    garf.position = glm::vec3(0.0, -3.5, -20.0);
    garf.rotationAxis = glm::vec3(1.0, 0.0, 0.0);
    garf.scale = 0.5;
    models.push_back(&garf);

    if(!loadModel(duckL, "model", "model/12248_Bird_v1_l2.obj")) return -1;
    duckL.position = glm::vec3(6.0, -8.5, -20.0);
    duckL.rotationAxis = glm::vec3(1.0, 0.0, 1.0);
    duckL.scale = 0.5;
    models.push_back(&duckL);

    if(!loadModel(duckR, "model", "model/12248_Bird_v1_l2.obj")) return -1;
    duckR.position = glm::vec3(-6.0, -8.5, -20.0);
    duckR.rotationAxis = glm::vec3(1.0, 0.0, -1.0);
    duckR.scale = 0.5;
    models.push_back(&duckR);

    if(!loadModel(eyeR, "model", "model/12221_Cat_v1_l3.obj")) return -1;
    eyeR.position = glm::vec3(-5.0, 7.0, -30.0);
    eyeR.scale = 0.1;
    models.push_back(&eyeR);

    if(!loadModel(eyeL, "model", "model/12221_Cat_v1_l3.obj")) return -1;
    eyeL.position = glm::vec3(5.0, 7.0, -30.0);
    eyeL.scale = 0.1;
    models.push_back(&eyeL);

    if(!loadModel(skull, "model", "model/12140_Skull_v3_L2.obj")) return -1;
    skull.position = glm::vec3(0.0, -10.5, -50.0);
    skull.scale = 0.65;
    models.push_back(&skull);

    return 0;
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
                                 glm::vec3(0.0f, -10.5f, -50.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    auto lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.0f, 100.0f);

    auto lightSpace = lightProjection * lightView;
    depthProgram.setLightSpace(lightSpace);



    auto shadowProgram = initShadowProgram();
    shadowProgram.use();

    shadowProgram.setLight(light);
    shadowProgram.setProjection(projection);
    shadowProgram.setLightSpace(lightSpace);
    shadowProgram.setDepthMap(1);



    std::vector<Model*> models;
    prepareModels(models);



    float bob = 0;
    float bol = 1;

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
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mode = 1;
        }
        if(glfwGetKey(window, GLFW_KEY_2)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mode = 2;
        }
        if(glfwGetKey(window, GLFW_KEY_3)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mode = 3;
        }

        if(bob >= 5.0) bol = -0.1;
        else if(bob <= 0.0) bol = 0.1;
        bob += bol;



        eyeL.position.y += bol;
        eyeR.position.y += bol;
        dino.rotation += (float) glfwGetTime() / 100;

        if(mode == 1) {
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glClearColor(1.0, 0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            program.use();

            program.setCamera(cam.cameraPos);
            program.setView(cam.getViewMatrix());

            for(Model *m : models) {
                m->render(&program);
            }
        }

        if(mode == 2 || mode == 3) {
            depthProgram.use();
            glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
            depthMap.bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            for(Model *m : models) {
                m->renderDepth(depthProgram);
            }

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

            for(Model *m : models) {
                m->render(&shadowProgram);
            }
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
