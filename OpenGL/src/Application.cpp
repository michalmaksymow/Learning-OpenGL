#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const char* glsl_version = "#version 130";

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Synchronize with v-sync */
    glfwSwapInterval(1);

    /* Initialize the GLEW library */
    if (glewInit() != GLEW_OK)
        return -1;

    /* Print out OpenGL version to the console */
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] =
        {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
            50.0f, -50.0f, 1.0f, 0.0f, // 1
            50.0f, 50.0f, 1.0f, 1.0f, // 2
            -50.0f, 50.0f, 0.0f, 1.0f  // 3
        };

        uint32_t indicies[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indicies, 6);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        

        Texture texture("res/textures/yt_logo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
        
        Renderer renderer;

        ImGui::CreateContext();
        IMGUI_CHECKVERSION();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
                renderer.Draw(va, ib, shader);
            }    
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
                renderer.Draw(va, ib, shader);
            }

            
            {
                ImGui::Begin("Hello, world!");
                ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return EXIT_SUCCESS;
}