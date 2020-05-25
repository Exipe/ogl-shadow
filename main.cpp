
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "cube.h"

Program initProgram() {
    auto vertex = compileShader("shader/vertex.glsl", GL_VERTEX_SHADER);
    auto fragment = compileShader("shader/fragment.glsl", GL_FRAGMENT_SHADER);

    auto program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    return linkProgram(program);
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

    //initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    auto camera = glm::vec3(0, 0, 3);

    auto program = initProgram();
    auto cube = initCube();

    program.use();

    auto model = glm::mat4(1.0);
    model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0, 1.0, 0.0));
    program.setModel(model);

    auto view = glm::mat4(1.0);
    view = glm::translate(view, -camera);
    program.setView(view);

    auto projection = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);
    program.setProjection(projection);

    Material mat { glm::vec3(1, 0, 0),
                   glm::vec3(0.5, 0, 0.1),
                   glm::vec3(0.5, 0.5, 0.5),
                   16.0};

    Light light {
        glm::vec3(5, 5, 5),
        glm::vec3(0.2, 0.2, 0.2),
        glm::vec3(0.5, 0.5, 0.5),
        glm::vec3(1.0, 1.0, 1.0)
    };

    program.setMaterial(mat);
    program.setLight(light);
    program.setCamera(camera);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.1, 0, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
