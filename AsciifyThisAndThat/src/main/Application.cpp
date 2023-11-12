/*
* Copyright 2023 Oğuzhan Topaloğlu
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include "Application.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <filesystem>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <imfilebrowser.h>


Application::Application() {
    current_ascii = new std::string();
    current_path = new std::string();
}


void Application::run() {

    if (!glfwInit()) exit(1);
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Asciify This & That", NULL, NULL);
    if (window == NULL)
        exit(1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    this->def_font = io.Fonts->AddFontDefault();
    this->font = io.Fonts->AddFontDefault();
    if (this->font == NULL || this->def_font == NULL) {
        std::cout << "font could not be loaded" << std::endl;
        exit(1);
    }

    // Our state
    ImVec4 clear_color = ImVec4(0, 0, 0, 1.00f);

    // file chooser
    ImGui::FileBrowser fileDialog;
    fileDialog.SetTitle("Choose an image");
    fileDialog.SetTypeFilters({ ".jpg", ".jpeg", ".png" });


    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // MAIN CODE
        ImGui::PushFont(this->font);
        if (ImGui::Begin("Path Window")) {
            static char* buf;
            std::string s = std::string(*(this->current_path));
            buf = &s[0];
            struct Func {
                static int MyCallback(ImGuiInputTextCallbackData* data) {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
                        std::string* curPath = (std::string*)data->UserData;
                        *curPath = std::string(data->Buf);
                        data->BufDirty = true;
                    }
                    return 0;
                }
            };
            if (ImGui::InputText("  Image Path", buf, 100, ImGuiInputTextFlags_CallbackEdit, 
                Func::MyCallback, (void*)this->current_path));
            if (ImGui::Button("Create Ascii Art")) {
                std::string* path = new std::string(buf);
                std::string* s = new std::string(twistral::get_ascifiied(*path));
                this->current_ascii = s;
            }
            ImGui::SameLine();
            if (ImGui::Button(" Open File Chooser ")) {
                fileDialog.Open();
            }
            ImGui::SameLine();
            ImGui::SliderFloat("Font Size", &this->def_font->FontSize, 6.0f, 20.0f);
        }
        ImGui::End();
        ImGui::PopFont();

        fileDialog.Display();
        if (fileDialog.HasSelected()) {
            current_path = new std::string(fileDialog.GetSelected().string());
            fileDialog.ClearSelected();
        }

        ImGui::PushFont(this->def_font);
        if (ImGui::Begin("Result Window")) {
            if (current_ascii->size() > 0) {
                ImGui::InputTextMultiline("##", current_ascii->data(), current_ascii->size() + 1, ImVec2({-FLT_MIN, -FLT_MIN }));
            }
        }
        ImGui::PopFont();
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}


