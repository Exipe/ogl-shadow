
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "cube.h"
#include "texture.h"
#include "model.h"
#include "Camera.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

Program initProgram() {
    auto vertex = compileShader("shader/vertex.glsl", GL_VERTEX_SHADER);
    auto fragment = compileShader("shader/fragment.glsl", GL_FRAGMENT_SHADER);

    auto program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    return linkProgram(program);
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

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    auto program = initProgram();
    program.use();

    auto skeleModel = glm::mat4(1.0);
    skeleModel = glm::translate(skeleModel, glm::vec3(0.0, 0.0, -40.0));
    skeleModel = glm::rotate(skeleModel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    skeleModel = glm::scale(skeleModel, glm::vec3(0.25, 0.25, 0.25));

    auto cubeModel = glm::mat4(1.0);
    cubeModel = glm::translate(cubeModel, glm::vec3(0, 0, -60));
    cubeModel = glm::scale(cubeModel, glm::vec3(20, 20, 5));

    auto projection = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);
    program.setProjection(projection);

    Light light {
        glm::vec3(0, 20, -20),
        glm::vec3(0.2, 0.2, 0.2),
        glm::vec3(0.5, 0.5, 0.5),
        glm::vec3(1.0, 1.0, 1.0)
    };
    program.setLight(light);

    auto cube = initCube();

    Model skull;
    if(!loadModel(skull, "model", "model/12140_Skull_v3_L2.obj")) {
        return -1;
    }

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

        glClearColor(0.1, 0, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.setCamera(cam.cameraPos);
        program.setView(cam.getViewMatrix());

        program.setModel(skeleModel);
        skull.render(program);

        program.setModel(cubeModel);
        cube.render(program);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
