
#include <Pandora/Entry.h>
#include <Pandora/App.h>

#include <Pandora/Libs/ImGui/imgui.h>
#include <Pandora/Libs/ImGuizmo/ImGuizmo.h>

using namespace pd;

class LitMaterial : public Material {
public:
    LitMaterial() {
        buffer = VideoAPI::Get()->CreateConstantBuffer();

        DataLayout layout;
        layout.Add("lightPos", DataLayoutType::Float3);
        layout.Add("viewPos", DataLayoutType::Float3);
        layout.Finish(DataPacking::Packed);

        buffer->SetBindSlot(1);
        buffer->SetBindTarget(ShaderType::Vertex);
        buffer->SetLayout(layout);
    }

    struct {
        alignas(16) Vec3 lightPos;
        alignas(16) Vec3 viewPos;
    } data;

protected:
    virtual void AfterBind() override {
        buffer->SetStruct(ToBytes(&data));
        buffer->Upload();
    }

private:
    Ref<ConstantBuffer> buffer;
};

class Game : public App {
public:
    Game(int argc, char** argv, VideoBackend backend) : App(argc, argv, backend, "Pandora Window", Vec2i(1280, 720)) {
        InitStorage("TomMol", "Sandbox");

        window.CaptureMouse(true);
        window.SetRelativeMouseMode(true);
        window.SetCursorVisible(false);

        // In a distributable build we want to use a box
        // catalog.Load("data.box");
        catalog.LoadFromConfig("data.json");

        cam.Init(window.GetSize(), 90.0f);
        cam.position = Vec3(0.5f, 1.0f, 5.5f);
        cam.forward = Vec3(0.0f, 0.0f, 1.0f);

        final.Init(window.GetSize());
        final.scale = 0.5f;

        cerberus.Load<LitMaterial>("Cerberus", "Shaders/Lit");
        cerberus.scale = 0.1f;

        mats.Add((LitMaterial*)cerberus.GetMaterial());
        mats.Last()->SetTexture("cerberus_A", 0);
        mats.Last()->SetTexture("cerberus_N", 1);

        plane.Load<LitMaterial>("Plane", "Shaders/Lit");
        plane.rotation.z = 180.0f;
        plane.position.z = -5.0f;
        plane.scale = Vec3(0.1f);

        mats.Add((LitMaterial*)plane.GetMaterial());
        mats.Last()->SetTexture("BricksAlbedo", 0);
        mats.Last()->SetTexture("BricksNormal", 1);

        text.Load("Osaka");
        text.text = "Hello, world!";
        text.scale = 0.01f;
        text.GenerateSprites();

        buffer = video->CreateFrameBuffer();
        buffer->Create(window.GetSize());

        bufferTex = buffer->GetTexture();
    }

    virtual ~Game() {}

protected:
    virtual void OnUpdate(f32 dt) override {
        cam.Update(dt, input, window);

        if (startupTimer > 0.0f) {
            startupTimer -= dt;
        }

        if (lockToCam) {
            lightPos = cam.position;
        }

        for (int i = 0; i < mats.Count(); i++) {
            mats[i]->data.viewPos = cam.position;
            mats[i]->data.lightPos = lightPos;
        }

        final.UpdateProjection();
    }

    virtual void OnRender(f32 dt) override {
        buffer->BindAndClear(video->GetClearColor());

        modelRenderer.UpdateProjection(cam.GetMatrix());

        modelRenderer.Draw(cerberus);
        modelRenderer.Draw(plane);

        spriteRenderer.UpdateProjection(cam);

        spriteRenderer.squishZ = false;
        text.Draw(spriteRenderer);

        spriteRenderer.Render();
    }

#if !defined(PD_NO_IMGUI)
    virtual void OnImGui() override {

        if (ImGui::Begin("Light")) {

            ImGui::Checkbox("Lock to camera", &lockToCam);

            if (!lockToCam) {
                ImGui::DragFloat3("Position", lightPos.elements, 0.1f);
            }

        }
        ImGui::End();

        if (ImGui::Begin("Text")) {
            char buffer[512];
            buffer[0] = '\0';

            memcpy(buffer, text.text.ByteData(), Min(text.text.Count() + 1, (int)sizeof(buffer)));

            if (ImGui::InputTextMultiline("Text", buffer, sizeof(buffer))) {
                text.text.Set(buffer);
                text.GenerateSprites();
            }

            if (ImGui::DragFloat3("Position", text.position.elements, 0.1f)) {
                text.UpdateProperties();
            }

            if (ImGui::DragFloat3("Scale", text.scale.elements, 0.1f)) {
                text.UpdateProperties();
            }

            if (ImGui::ColorEdit4("Color", text.color.elements, 0.1f)) {
                text.UpdateProperties();
            }
        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::Begin("Game")) {
            ImVec2 minRegion = ImGui::GetWindowContentRegionMin();
            ImVec2 maxRegion = ImGui::GetWindowContentRegionMax();

            ImVec2 windowSize = ImVec2(maxRegion.x - minRegion.x, maxRegion.y - minRegion.y);

            ImVec2 size = ImVec2(bufferTex->GetSize().x, bufferTex->GetSize().y);

            float scale = Min(windowSize.x / size.x, windowSize.y / size.y);

            ImGui::Image(bufferTex->GetImGuiTextureID(), ImVec2(size.x * scale, size.y * scale),
                         ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

            // Gizmos
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            f32 width = ImGui::GetWindowWidth();
            f32 height = ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

            Mat4 cameraProj = cam.GetProjectionMatrix();
            Mat4 cameraView = cam.GetViewMatrix();

            static bool selectedCerberus = true;

            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
                selectedCerberus = !selectedCerberus;
            }

            Mat4 matrix = (selectedCerberus) ? cerberus.GetMatrix() : plane.GetMatrix();

            static ImGuizmo::OPERATION op = ImGuizmo::TRANSLATE;

            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))) {
                op = ImGuizmo::TRANSLATE;
            }

            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
                op = ImGuizmo::ROTATE;
            }

            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow))) {
                op = ImGuizmo::SCALE;
            }

            ImGuizmo::Manipulate(&cameraView[0][0], &cameraProj[0][0], op, ImGuizmo::LOCAL,
                                 &matrix[0][0]);

            if (ImGuizmo::IsUsing()) {
                Vec3 trans, scale;
                Quat orient;
                Decompose(matrix, &scale, &orient, &trans);

                if (selectedCerberus) {
                    cerberus.position = trans;
                    cerberus.rotation = orient;
                    cerberus.scale = scale;
                } else {
                    plane.position = trans;
                    plane.rotation = orient;
                    plane.scale = scale;
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
#endif

    virtual void OnEvent(Event* event) override {
        if (startupTimer <= 0.0f) {
            cam.HandleInput(event);
        }
    }

    FPSCamera cam;
    Camera final;

    // Timer to wait for input
    f32 startupTimer = 0.016f;

    Vec3 lightPos = Vec3(0.5f, 1.0f, 0.3f);
    bool lockToCam = true;

    SpriteRenderer spriteRenderer;
    ModelRenderer modelRenderer;

    Model cerberus;
    Model plane;

    Text text;

    Array<LitMaterial*> mats;

    Ref<FrameBuffer> buffer;
    Ref<Texture> bufferTex;
};

App* pd::CreateApp(int argc, char** argv) {
    VideoBackend backend = VideoBackend::OpenGL;

    JsonParseSettings settings;
    settings.allowComments = true;

    JsonValue json;
    if (json.Parse("config.json", true, settings) && json.Type() == JsonType::Object && json.HasField("backend")) {
        JsonValue back = json["backend"];
        if (back.Type() == JsonType::String) {
            for (int i = 0; i < (int)VideoBackend::Count; i++) {

                if (back.GetString() == pd::VIDEO_BACKEND_NAMES[i]) {
                    backend = (VideoBackend)i;
                    break;
                }
            }
        }
    }

    return New<Game>(argc, argv, backend);
}
