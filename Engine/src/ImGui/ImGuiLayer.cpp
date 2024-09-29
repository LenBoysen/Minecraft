#include "ImGuiLayer.h"

#include"Window.h"
#include <iostream>
//Temp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#define BIND_EVENT_FN(x) std::bind(&ImGuiLayer::x, this, std::placeholders::_1)


ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer"){

}

ImGuiLayer::~ImGuiLayer(){

}
 void ImGuiLayer::onAttach() {
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    try {
        io.Fonts->AddFontFromFileTTF("C:\\Users\\1\\AppData\\Local\\Microsoft\\Windows\\Fonts\\CaskaydiaCoveNerdFont-Regular.ttf", 18.5f, NULL, NULL);
    }
    catch(...){
        std::cerr << "Font could not load";
    }

    // Setup Platform/Renderer backends
    Application& app = Application::Get();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().GetNativeWindow());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}
void ImGuiLayer::onDetach(){
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::onImGuiRender()
{
    //static bool show = true;
    //ImGui::ShowDemoWindow(&show);
}

void ImGuiLayer::begin(){
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void ImGuiLayer::end() {

    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().GetNativeWindow());
    io.DisplaySize = ImVec2((float)app.getWindow().GetWidth(), (float)app.getWindow().GetWidth());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}


