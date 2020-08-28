#if !defined(PD_NO_OPENGL)
#include "GLVideoAPI.h"

#include "Pandora/Core/IO/Console.h"

#include "Pandora/Graphics/Backend/OpenGL/GLShader.h"
#include "Pandora/Graphics/Backend/OpenGL/GLRenderer.h"
#include "Pandora/Graphics/Backend/OpenGL/GLTexture.h"
#include "Pandora/Graphics/Backend/OpenGL/GLConstantBuffer.h"
#include "Pandora/Graphics/Backend/OpenGL/GLFrameBuffer.h"

#include "Pandora/Graphics/Backend/FreeType/FTFont.h"

#define GLAD_WGL_IMPLEMENTATION
#include "Pandora/Libs/glad/wgl.h"
#include "Pandora/Libs/glad/glad.h"

#include "Pandora/Libs/ImGui/OpenGL/imgui_impl_opengl3.h"


#if defined(PD_WINDOWS)
#include <Windows.h>
#include "Pandora/Libs/ImGui/imgui_impl_win32.h"
#endif

#pragma comment(lib, "OpenGL32.lib")

namespace pd {

void* GLLoadProc(const char* name) {
    // Try loading it with wglGetProcAddress
    PROC proc = wglGetProcAddress(name);

    // Otherwise try OpenGL32.dll
    if (!proc ||
        (proc == (void*)0x1) || (proc == (void*)0x2) || (proc == (void*)0x3) || (proc == (void*)-1)) {
        static HMODULE openGL = GetModuleHandleW(L"OpenGL32.dll");
        proc = GetProcAddress(openGL, name);
    }

    return (void*)proc;
}

GLADapiproc WGLLoadProc(const char* name) {
    return (GLADapiproc)GLLoadProc(name);
}

GLVideoAPI::GLVideoAPI(Window* window) :
    VideoAPI(window) {

    InitOpenGLExtensions();

#if defined(PD_WINDOWS)
    // Create OpenGL surface
    windowDevice = GetDC((HWND)window->GetNativeHandle());
    HDC currentHDC = (HDC)windowDevice;

    int format = 0;
    u32 formatCount = 0;

    int formatAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
        0, 0
    };

    wglChoosePixelFormatARB(currentHDC, formatAttribs, 0, 1, &format, &formatCount);

    PD_ASSERT_D(formatCount > 0, "failed to set the OpenGL pixel format");

    PIXELFORMATDESCRIPTOR pfd = {};
    DescribePixelFormat(currentHDC, format, sizeof(pfd), &pfd);

    bool success = SetPixelFormat(currentHDC, format, &pfd);
    PD_ASSERT_D(success, "couldn't set pixel format");

    int glVersion[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0, 0
    };

    glContext = wglCreateContextAttribsARB(currentHDC, NULL, glVersion);

    PD_ASSERT_D(glContext, "failed to create OpenGL context");

    success = wglMakeCurrent(currentHDC, (HGLRC)glContext);
    PD_ASSERT_D(success, "failed to make OpenGL context current");

#else
#error Implement me!
#endif

    gladLoadGLLoader(GLLoadProc);

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

#if !defined(PD_NO_IMGUI)
    // Initialize ImGui
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();

    struct Handles {
        HGLRC context;
        HDC device;
    };

    Handles* imHandles = New<Handles>();

    imHandles->context = (HGLRC)glContext;
    imHandles->device = (HDC)windowDevice;

    platformIO.MainViewport->RendererUserData = imHandles;

    platformIO.Platform_RenderWindow = [](ImGuiViewport* vp, void* renderArg) {
        wglMakeCurrent(GetDC((HWND)vp->PlatformHandle), (HGLRC)renderArg);
    };

    platformIO.Renderer_SwapBuffers = [](ImGuiViewport* vp, void* renderArg) {
        wglMakeCurrent(GetDC((HWND)vp->PlatformHandle), (HGLRC)renderArg);
        wglSwapLayerBuffers(GetDC((HWND)vp->PlatformHandle), WGL_SWAP_MAIN_PLANE);
    };
#endif

    // Initialize the renderer
    renderer.Reset(New<GLRenderer>());
}

void GLVideoAPI::InitOpenGLExtensions() {
#if defined(PD_WINDOWS)
    WNDCLASSW dummyClass = {};
    dummyClass.style = CS_HREDRAW | CS_VREDRAW;
    dummyClass.lpfnWndProc = DefWindowProcW;
    dummyClass.hInstance = GetModuleHandleW(NULL);
    dummyClass.lpszClassName = L"PANDORA_OPENGL_DUMMY";

    bool success = RegisterClassW(&dummyClass);
    PD_ASSERT_D(success, "could not register OpenGL dummy window class");

    HWND dummyWindow = CreateWindowW(dummyClass.lpszClassName, L"Pandora OpenGL Dummy", 0,
                                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                     NULL, NULL, dummyClass.hInstance, NULL);

    PD_ASSERT_D(dummyWindow != NULL, "failed to create OpenGL dummy window");

    HDC dummyHDC = GetDC(dummyWindow);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int format = ChoosePixelFormat(dummyHDC, &pfd);

    success = SetPixelFormat(dummyHDC, format, &pfd);
    PD_ASSERT_D(success, "failed to set pixel format");

    HGLRC tempContext = wglCreateContext(dummyHDC);

    PD_ASSERT_D(tempContext, "failed to create OpenGL dummy context");

    success = wglMakeCurrent(dummyHDC, tempContext);
    PD_ASSERT_D(success, "failed to make OpenGL dummy context current");

    // Load extensions
    gladLoadWGL(dummyHDC, WGLLoadProc);
    gladLoadGLLoader(GLLoadProc);

    CONSOLE_LOG_DEBUG("  {}> {}\n  > {}{}\n", ConColor::Grey, (char*)glGetString(GL_VENDOR),
        (char*)glGetString(GL_VERSION), ConColor::White);

    // Delete our temporary context
    wglDeleteContext(tempContext);

    ReleaseDC(dummyWindow, dummyHDC);
    DestroyWindow(dummyWindow);
#else
#error Implement me!
#endif
}

GLVideoAPI::~GLVideoAPI() {
    if (glContext) {
        wglDeleteContext((HGLRC)glContext);
    }

#if !defined(PD_NO_IMGUI)
    pd::Delete(ImGui::GetMainViewport()->RendererUserData);
    ImGui_ImplOpenGL3_Shutdown();
#endif
}

void GLVideoAPI::SetClearColor(Color color) {
    clearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}

void GLVideoAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLVideoAPI::Swap() {
#if defined(PD_WINDOWS)
    SetClearColor(clearColor);
    wglSwapLayerBuffers((HDC)windowDevice, WGL_SWAP_MAIN_PLANE);
#else
#error Implement me!
#endif
}

void GLVideoAPI::SetVsync(bool enabled) {
#if defined(PD_WINDOWS)
    wglSwapIntervalEXT((enabled) ? 1 : 0);
#else
#error Implement me!
#endif
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
    ImGui::RenderPlatformWindowsDefault(glContext);

    // Updating the windows might change the current context
#if defined(PD_WINDOWS)
    wglMakeCurrent((HDC)windowDevice, (HGLRC)glContext);
#else
#error Implement me!
#endif
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
