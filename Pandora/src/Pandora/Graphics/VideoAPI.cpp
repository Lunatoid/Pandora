#include "VideoAPI.h"

#if !defined(PD_NO_OPENGL)
#include "Pandora/Graphics/Backend/OpenGL/GLVideoAPI.h"
#endif

#include "Pandora/Core/IO/Console.h"

#include "Pandora/Graphics/Model/Mesh.h"

namespace pd {

// @GLOBAL
static VideoAPI* globalVideoAPI = nullptr;

VideoAPI::VideoAPI(Window* window) : window(window) {}

VideoAPI* VideoAPI::Create(Window* window, VideoBackend backend) {
    PD_ASSERT_D(!globalVideoAPI, "creation of multiple video APIs not allowed");

    CONSOLE_LOG_DEBUG("  {}> Creating {} backend{}\n", ConColor::Grey, backend, ConColor::White);

    if (backend == VideoBackend::OpenGL) {
#if !defined(PD_NO_OPENGL)
        globalVideoAPI = New<GLVideoAPI>(window);
#else
        PD_ASSERT(false, "Backend %s it not available", VIDEO_BACKEND_NAMES[(int)backend].Data());
#endif
    }

    globalVideoAPI->Init();
    return globalVideoAPI;
}

VideoAPI* VideoAPI::Get() {
    PD_ASSERT_D(globalVideoAPI, "no video API created");
    return globalVideoAPI;
}

void VideoAPI::Delete() {
    if (globalVideoAPI) {
        pd::Delete(globalVideoAPI);
    }
}

Color VideoAPI::GetClearColor() const {
    return clearColor;
}

void VideoAPI::SetMeshRequestHandler(ResourceCatalog& catalog) {
    catalog.SetResourceRequestHandler(ResourceType::Mesh, [](Box& box, ResourceType type, StringView name, void* data) {
        Mesh* mesh = New<Mesh>();

        if (!mesh->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load mesh {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)mesh;
    });
}

Ref<Renderer> VideoAPI::GetRenderer() {
    return renderer;
}

}
