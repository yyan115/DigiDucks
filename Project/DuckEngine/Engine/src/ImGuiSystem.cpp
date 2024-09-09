//#include "ImGuiSystem.h"
//
//ImGuiSystem::ImGuiSystem(GraphicsManager* graphicsManager) : graphicsManager(graphicsManager) {}
//
//ImGuiSystem::~ImGuiSystem() {
//    Shutdown();
//}
//
//void ImGuiSystem::Initialize() {
//    // Initialize ImGui context
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGui::StyleColorsDark();
//
//    // Initialize ImGui backends (GLFW and OpenGL)
//    ImGui_ImplGlfw_InitForOpenGL(graphicsManager->GetWindow(), true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//}
//
//void ImGuiSystem::Update() {
//    // Start ImGui frame
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//
//    // Here you can add custom ImGui windows for debugging or settings
//    ImGui::Begin("Example Window");
//    ImGui::Text("Hello, ImGui!");
//    ImGui::End();
//
//    // Render ImGui on top of the scene
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}
//
//void ImGuiSystem::Shutdown() {
//    // Clean up ImGui resources
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//}
