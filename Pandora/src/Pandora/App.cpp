#include "Pandora/App.h"

#include "Core/IO/Path.h"
#include "Pandora/Libs/ImGui/imgui.h"
#include "Pandora/Libs/ImGuizmo/ImGuizmo.h"

#include <SDL2/SDL.h>

namespace pd {

const int SDL_SUBSYSTEMS = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER;

App::App(int argc, char** argv, VideoBackend backend, StringView title, Vec2i size, u32 style) :
    catalog(ResourceCatalog::Get()) {
    // General initialization
    for (int i = 0; i < argc; i++) {
        args.Add(argv[i]);
    }

    random.Seed();
    if (SDL_Init(SDL_SUBSYSTEMS) != 0) {
        console.Log("[{}SDL Error{}] couldn't initialize SDL: {}\n", ConColor::Red, ConColor::White, SDL_GetError());
    }

    video = VideoAPI::Create(&window, backend);

    // Open window and create video backend+renderer
    window.Open(title, size, style, backend);

    if (!video->Load()) {
        return;
    }

    PD_ASSERT_D(video, "video backend could not be created (backend '%s')", VIDEO_BACKEND_NAMES[(int)backend].Data());

    video->SetVsync(true);
    video->SetViewport(window.GetSize());
    video->SetClearColor(Color(0.1f, 0.1f, 0.1f));

    renderer = video->GetRenderer();

    // Create audio backend
    audio = AudioAPI::Create(AudioBackend::SoLoud);

    // Initialize all handlers
    video->SetTextureRequestHandler(catalog);
    video->SetShaderRequestHandler(catalog);
    video->SetFontRequestHandler(catalog);
    video->SetMeshRequestHandler(catalog);
    audio->SetAudioRequestHandler(catalog);

#if !defined(PD_NO_IMGUI)
    LoadImGuiTheme();
#endif
}

App::~App() {
    // We want to free the resources here because some of the destruction
    // might depend on the AudioAPI/VideoAPI, however this might give us
    // some false positives in regards to some of the resources because
    // the App is not fully destructed yet.
    //
    // @TODO: find a way around calling the destructor manually, better
    // order of destruction to minimize false positives.
    catalog.~ResourceCatalog();

    VideoAPI::Delete();
    AudioAPI::Delete();
    window.Delete();
    DeleteStorage();

    SDL_Quit();
}

void App::Run() {
    if (IsRunning()) return;

#if !defined(PD_NO_IMGUI)
    // Attempt to Load ImGui font if we have any specified
    // We don't do this in the constructor because the catalog will not have loaded anything yet
    if (catalog.GetResourceData("Fonts/ImGui", fontData)) {
        ImFontConfig config = {};
        strcpy(config.Name, "ImGuiFont");
        config.FontDataOwnedByAtlas = false;
        imguiFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(fontData.Data(), fontData.Count(), 16.0f, &config);
    }
#endif

    isRunning = true;

    Stopwatch deltaClock;

    while (IsRunning()) {
        rawDelta = (f32)deltaClock.Restart().seconds;

        OnUpdateInternal(rawDelta * updateTimescale);

        video->BindDefaultFrameBuffer();
        OnRenderInternal(rawDelta * renderTimescale);
        video->BindDefaultFrameBuffer();

#if !defined(PD_NO_IMGUI)
        OnImGuiInternal();
#endif

        video->Swap();
    }

    OnQuit();
}

void App::Quit() {
    isRunning = false;
}

bool App::IsRunning() const {
    return isRunning;
}

void App::PushEvent(Event* event) {
    OnEventInternal(event);
}

Vec2 App::GetNormalizedMousePos() {
    Vec2i windowSize = window.GetSize();

    Vec2 mousePos = GetMousePos();
    Vec2 normalized = Vec2((f32)mousePos.x, (f32)mousePos.y) / Vec2((f32)windowSize.x, (f32)windowSize.y);
    normalized *= Vec2(2.0f);
    normalized -= Vec2(1.0f);

    return normalized;
}

Vec2 App::GetMousePos() const {
    return input.GetMousePos();
}

Window& App::GetWindow() {
    return window;
}

InputManager& App::GetInputManager() {
    return input;
}

Ref<Renderer> App::GetRenderer(RefType type) {
    return renderer.NewRef(type);
}

void App::OnUpdateInternal(f32 dt) {
    window.HandleEvents();
    input.Update();

    WindowEvent event;
    while (window.PollEvent(&event)) {
        PushEvent(&event);
    }

    OnUpdate(dt);
}

void App::OnRenderInternal(f32 dt) {
    video->Clear();

    OnRender(dt);
}

#if !defined(PD_NO_IMGUI)

void App::OnImGuiInternal() {
    // Explicitly check if we're quitting because otherwise we abort
    if (!IsRunning()) return;

    video->ImGuiNewFrame();
    window.ImGuiNewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    if (!window.IsCursorVisible()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    }

    // Enable dockspace
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

    if (imguiFont) {
        ImGui::PushFont(imguiFont);
    }

    OnImGui();

    if (imguiFont) {
        ImGui::PopFont();
    }

    ImGui::Render();
    video->ImGuiRender();
}

void App::LoadImGuiTheme() {
    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
    ImGui::GetStyle().GrabRounding = 2;
    ImGui::GetStyle().ScrollbarRounding = 2;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.45f, 0.57f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.57f, 0.63f, 0.73f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.44f, 0.31f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.60f, 0.29f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.60f, 0.29f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.35f, 0.77f, 0.31f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.52f, 0.52f, 0.52f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.57f, 0.63f, 0.73f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.30f, 0.43f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.52f, 0.52f, 0.52f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.78f, 0.81f, 0.87f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.45f, 0.57f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.57f, 0.63f, 0.73f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.60f, 0.29f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.94f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.60f, 0.29f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.77f, 0.31f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.77f, 0.31f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.30f, 0.30f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12f, 0.44f, 0.31f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.35f, 0.77f, 0.31f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.92f, 0.34f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.64f, 0.08f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.92f, 0.34f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.80f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.92f, 0.34f, 0.90f);
}

#endif

void App::OnEventInternal(Event* event) {
    // Handle certain events
    if (event->category == EventCategory::Window) {
        WindowEvent* we = (WindowEvent*)event;

        switch (we->type) {
            case WindowEventType::Close: {
                Quit();
                break;
            }
        }
    }
    input.OnEvent(event);

    // Pass the events to the user
    OnEvent(event);
}

}
