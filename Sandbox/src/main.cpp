
#include <Pandora/Entry.h>

#include <Pandora/Libs/ImGui/imgui.h>

using namespace pd;

struct {
    alignas(16) Vec3 lightPos = Vec3(0.0f);
    alignas(16) Vec3 materialColor = Vec3(1.0f);
    alignas(16) Vec3 ambient = Vec3(0.15f);
    alignas(16) Vec3 diffuseColor = Vec3(1.0f);
    f32 diffuseIntensity = 1.0f;
    f32 attConst = 1.0f;
    f32 attLin = 0.045f;
    f32 attQuad = 0.0075f;
} light;

class PhongMaterial : public Material {
public:
    PhongMaterial() {
        lightBuffer = VideoAPI::Get()->CreateConstantBuffer();

        DataLayout layout(true);
        layout.Add("lightPos", DataLayoutType::Float3);
        layout.Add("materialColor", DataLayoutType::Float3);
        layout.Add("ambient", DataLayoutType::Float3);
        layout.Add("diffuseColor", DataLayoutType::Float3);
        layout.Add("diffuseIntensity", DataLayoutType::Float);
        layout.Add("attConst", DataLayoutType::Float);
        layout.Add("attLin", DataLayoutType::Float);
        layout.Add("attQuad", DataLayoutType::Float);
        layout.Finish();

        lightBuffer->SetLayout(layout);
        lightBuffer->SetBindTarget(ShaderType::Pixel);
    }
protected:
    virtual void AfterBind() override {
        lightBuffer->SetStruct(ToBytes(&light));
        lightBuffer->Upload();
    }

private:
    Ref<ConstantBuffer> lightBuffer;
};

class Game : public App {
public:
    Game(int argc, char** argv, VideoBackend backend) : App(argc, argv, backend, "Pandora Window", Vec2i(1280, 720)) {
        InitStorage("TomMol", "Sandbox");

        window.CaptureMouse(true);
        window.SetRelativeMouseMode(true);
        window.SetCursorVisible(false);

        catalog.Load("data.box");

        cam.Init(window.GetSize(), 90.0f);
        cam.position = Vec3(0.5f, 1.0f, 5.5f);
        cam.forward = Vec3(0.0f, 0.0f, 1.0f);

        final.Init(window.GetSize());
        final.scale = 0.5f;

        tiles.Load(Vec2(0.0f), Vec2(32.0f), "tiles");
        tiles.baseSprite.position.xy = Vec2(-16.0f);
        tiles.baseSprite.position.z = -1.0f;
        tiles.baseSprite.color = Color(0.5f);

        helloText.Load("OsakaFont");
        helloText.text.Set(u8"おはよう、Unicode!");
        helloText.scale = 0.01f;
        helloText.position.y = 1.0f;
        helloText.GenerateSprites();
        helloText.position.x -= helloText.CalculateBounds().x / 2.0f;
        helloText.UpdateProperties();

        emojiText.Load("SegoeFont");
        emojiText.text.Set(u8"🙂🤝📈🌷⛩☣");
        emojiText.scale = 0.01f;
        emojiText.position = Vec2(-emojiText.CalculateBounds().x / 2.0f, -3.0f);
        emojiText.GenerateSprites();

        monkeModel.Load("monke", "Shaders/Phong");
        monkeModel.position.x = -3.0f;
        monkeModel.CreateNewMaterial<PhongMaterial>("Shaders/Phong");
        monkeMat = (PhongMaterial*)monkeModel.GetMaterial();
        monkeMat->SetTexture("monkeTex", 0);

        negaMonkeModel.Load("monke", "Shaders/Phong");
        negaMonkeModel.position.x = 3.0f;
        negaMonkeModel.CreateNewMaterial<PhongMaterial>("Shaders/Phong");
        negaMonkeMat = (PhongMaterial*)negaMonkeModel.GetMaterial();
        negaMonkeMat->SetTexture("monkeTexInv", 0);

        monkeys = video->CreateFrameBuffer();
        monkeys->Create(window.GetSize());
        monkeysSprite.Load(monkeys->GetTexture());
        
        rest = video->CreateFrameBuffer();
        rest->Create(window.GetSize());
        restSprite.Load(rest->GetTexture());
        
        monkeysSprite.position.x -= 1280.0f;
        monkeysSprite.position.z = 1.0f;
    }

    virtual ~Game() {}

protected:
    virtual void OnUpdate(f32 dt) override {
        cam.Update(dt, input, window);
        light.lightPos = cam.position;

        monkeModel.rotation.y = (f32)Sin((f64)GetTicks());
        monkeModel.rotation.z += dt * 100.0f;

        negaMonkeModel.position.z = (f32)Sin((f64)GetTicks() / 1000.0) * 0.5f;
        negaMonkeModel.position.x = 3.0f + (f32)Sin((f64)GetTicks() / 500.0);

        switchTileCounter += dt;
        if (switchTileCounter > 1.0f) {
            currentSprite += 1;
            switchTileCounter = 0.0f;
        }

        if (startupTimer > 0.0f) {
            startupTimer -= dt;
        }

        final.UpdateProjection();
    }

    virtual void OnRender(f32 dt) override {
        monkeys->BindAndClear(Color(1.0f, 1.0f));
        
        // Render the models before the sprites because the textures could have alpha
        modelRenderer.UpdateProjection(cam.GetMatrix());
        modelRenderer.Draw(monkeModel);
        modelRenderer.Draw(negaMonkeModel);
        
        rest->BindAndClear(Color(1.0f, 0.0f, 1.0f, 1.0f));
        
        // We're using a perspective camera so we won't squish the Z
        spriteRenderer.squishZ = false;
        
        spriteRenderer.UpdateProjection(cam.GetMatrix());
        spriteRenderer.Draw(*tiles[0][currentSprite]);
        helloText.Draw(spriteRenderer);
        emojiText.Draw(spriteRenderer);
        
        spriteRenderer.Render();        
    }

#if !defined(PD_NO_IMGUI)
    virtual void OnImGui() override {
        Ref<Texture> monkeyTex = monkeys->GetTexture(RefType::Weak);
        Ref<Texture> restTex = rest->GetTexture(RefType::Weak);
        
        ImVec2 size = ImVec2(1280/4, 720/4);

        ImGui::Begin("Monkeys");
        ImGui::Image(monkeyTex->GetImGuiTextureID(), size);
        ImGui::End();
        
        ImGui::Begin("Rest");
        ImGui::Image(restTex->GetImGuiTextureID(), size);
        ImGui::End();

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

    SpriteRenderer spriteRenderer;

    SpriteAtlas tiles;
    int currentSprite = 0;
    f32 switchTileCounter = 0.0f;

    Text helloText;
    Text emojiText;

    ModelRenderer modelRenderer;
    Model monkeModel;
    Model negaMonkeModel;
    PhongMaterial* monkeMat = nullptr;
    PhongMaterial* negaMonkeMat = nullptr;

    Ref<FrameBuffer> monkeys;
    Sprite monkeysSprite;

    Ref<FrameBuffer> rest;
    Sprite restSprite;
};

App* pd::CreateApp(int argc, char** argv) {

    VideoBackend backend = VideoBackend::DirectX;

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
