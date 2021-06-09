#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#if !defined(PD_NO_OPENGL)
#include "GLVideoAPI.h"

#include "Pandora/Core/IO/Console.h"

#include "Pandora/Graphics/Backend/OpenGL/GLShader.h"
#include "Pandora/Graphics/Backend/OpenGL/GLRenderer.h"
#include "Pandora/Graphics/Backend/OpenGL/GLTexture.h"
#include "Pandora/Graphics/Backend/OpenGL/GLConstantBuffer.h"
#include "Pandora/Graphics/Backend/OpenGL/GLFrameBuffer.h"

#include "Pandora/Graphics/Backend/FreeType/FTFont.h"

#include "Pandora/Libs/glad/glad.h"

#include "Pandora/Libs/ImGui/OpenGL/imgui_impl_opengl3.h"

#include <SDL2/SDL.h>

#if defined(PD_WINDOWS)
#pragma comment(lib, "OpenGL32.lib")
#endif


#if !defined(PD_NO_IMGUI)
#include "Pandora/Libs/ImGui/SDL2/imgui_impl_sdl.h"
#endif

const int PD_OPENGL_MAJOR_VER = 3;
const int PD_OPENGL_MINOR_VER = 3;

namespace pd {

GLVideoAPI::GLVideoAPI(Window* window) : VideoAPI(window) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#if defined(PD_DEBUG)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
}

GLVideoAPI::~GLVideoAPI() {
    if (nativeData) {
        SDL_GL_DeleteContext((SDL_GLContext)nativeData);
    }

#if !defined(PD_NO_IMGUI)
    // pd::Delete(ImGui::GetMainViewport()->RendererUserData);
    ImGui_ImplOpenGL3_Shutdown();
#endif
}

bool GLVideoAPI::Load() {

    nativeData = SDL_GL_CreateContext((SDL_Window*)window->GetNativeHandle());

    if (!nativeData) {
        console.Log("[{}GL Error{}] could not create SDL GL context: {}\n", ConColor::Red, ConColor::White, SDL_GetError());
        return false;
    }

    
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        console.Log("[{}GL Error{}] couldn't load OpenGL procedures: {}\n", ConColor::Red, ConColor::White, SDL_GetError());
        return false;
    }

    /*
    if (!SDL_GL_MakeCurrent((SDL_Window*)window->GetNativeHandle(), (SDL_GLContext)nativeData)) {
        console.Log("[{}GL Error{}] could not make SDL GL context current: {}\n", ConColor::Red, ConColor::White, SDL_GetError());
        return false;
    }
    */

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glFrontFace(GL_CCW);
    
    auto reportOpenGLError = [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam) {
        if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
            CONSOLE_LOG_DEBUG("[{}GL Error{}] {}\n", ConColor::Red, ConColor::White, message);
        }
    };

#if defined(PD_DEBUG)
    glEnable(GL_DEBUG_OUTPUT);
    if (glDebugMessageCallback) {
        glDebugMessageCallback(reportOpenGLError, NULL);
    } else {
        glDebugMessageCallbackARB(reportOpenGLError, NULL);
    }
#endif

    glViewport(0, 0, window->GetSize().x, window->GetSize().y);
    
    // Initialize the renderer
    renderer.Reset(New<GLRenderer>());

    // Initialize the ImGui context
#if !defined(PD_NO_IMGUI)
    ImGui::CreateContext();

    // Set up config BEFORE initializing backends
    ImGuiIO& io = ImGui::GetIO();
    io.RenderDrawListsFn = nullptr; // For some reason this was not always set to nullptr
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window->GetNativeHandle(), nativeData);
    ImGui_ImplOpenGL3_Init("#version 330");

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& imStyle = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        imStyle.WindowRounding = 0.0f;
        imStyle.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif

    return true;
}

void GLVideoAPI::SetClearColor(Color color) {
    clearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}

void GLVideoAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLVideoAPI::Swap() {
    SetClearColor(clearColor);
    SDL_GL_SwapWindow((SDL_Window*)window->GetNativeHandle());
}

void GLVideoAPI::SetVsync(bool enabled) {
    SDL_GL_SetSwapInterval((enabled) ? 1 : 0);
}

void GLVideoAPI::SetViewport(Vec2i size) {
    glViewport(0, 0, size.x, size.y);
}

void GLVideoAPI::BindDefaultFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#if !defined(PD_NO_IMGUI)
void GLVideoAPI::ImGuiNewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
}

void GLVideoAPI::ImGuiRender() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}
#endif

Ref<Shader> GLVideoAPI::CreateShader() {
    return Ref<Shader>(New<GLShader>());
}

Ref<Texture> GLVideoAPI::CreateTexture() {
    return Ref<Texture>(New<GLTexture>());
}

// I LOVE WINDOWS.H
#pragma push_macro("CreateFont")
#undef CreateFont
Ref<Font> GLVideoAPI::CreateFont() {
    return Ref<Font>(New<FTFont>());
}
#define CreateFont

Ref<ConstantBuffer> GLVideoAPI::CreateConstantBuffer() {
    return Ref<ConstantBuffer>(New<GLConstantBuffer>());
}

Ref<FrameBuffer> GLVideoAPI::CreateFrameBuffer() {
    return Ref<FrameBuffer>(New<GLFrameBuffer>());
}

void GLVideoAPI::SetTextureRequestHandler(ResourceCatalog& catalog) {
    catalog.SetResourceRequestHandler(ResourceType::Texture, [](Box& box, ResourceType type, StringView name, void* data) {
        GLTexture* tex = New<GLTexture>();

        // Apply import options
        GLVideoAPI* video = (GLVideoAPI*)data;
        tex->filtering = video->textureOptions.filtering;
        tex->wrapping = video->textureOptions.wrapping;

        if (!tex->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load texture {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)tex;
    }, this);
}

void GLVideoAPI::SetShaderRequestHandler(ResourceCatalog& catalog) {
    catalog.SetResourceRequestHandler(ResourceType::Shader, [](Box& box, ResourceType type, StringView name, void* data) {
        GLShader* shader = New<GLShader>();

        if (!shader->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load shader {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)shader;
    });
}

void GLVideoAPI::SetFontRequestHandler(ResourceCatalog& catalog) {
    catalog.SetResourceRequestHandler(ResourceType::Font, [](Box& box, ResourceType type, StringView name, void* data) {
        FTFont* font = New<FTFont>();

        if (!font->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load font {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)font;
    });
}

void GLVideoAPI::Init() {
    // Nothing
}

}

#endif
