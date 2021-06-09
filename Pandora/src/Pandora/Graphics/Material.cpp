#include "Material.h"

#include "Pandora/Core/IO/Console.h"

#include "Pandora/Graphics/VideoAPI.h"

namespace pd {

Material::Material() : catalog(ResourceCatalog::Get()) {
    textures.Reserve(PD_MAX_TEXTURE_UNITS);
}

void Material::Load(StringView shaderName, DataLayout& layout) {
    shader = catalog.Get<Shader>(shaderName);
    shader->SetLayout(layout);

    AfterLoad();
}

void Material::Bind(Renderer* renderer) {
    BeforeBind();

    renderer->SetShader(shader);
    renderer->BindShader();

    for (int i = 0; i < textures.Count(); i++) {
        if (textures[i]) {
            textures[i]->Bind(i);
        }
    }

    AfterBind();
}

void Material::SetTexture(StringView name, int slot) {
    if (slot < 0 || slot >= PD_MAX_TEXTURE_UNITS) {
        CONSOLE_LOG_DEBUG("[{}Material Error{}] {} is not a valid texture slot (min. 0, max. {})\n",
            ConColor::Red, ConColor::White, slot, PD_MAX_TEXTURE_UNITS);
        return;
    }

    textures[slot] = catalog.Get<Texture>(name);
    shader->ActivateTextureSlot(slot);

    AfterSetTexture(slot);
}

void Material::SetTexture(const Ref<Texture>& texture, int slot) {
    if (slot < 0 || slot >= PD_MAX_TEXTURE_UNITS) {
        CONSOLE_LOG_DEBUG("[{}Material Error{}] {} is not a valid texture slot (min. 0, max. {})\n",
            ConColor::Red, ConColor::White, slot, PD_MAX_TEXTURE_UNITS);
        return;
    }

    textures[slot] = texture;
    shader->ActivateTextureSlot(slot);

    AfterSetTexture(slot);
}

Ref<Texture> Material::GetTexture(int slot, RefType type) const {
    return textures[slot].NewRef(type);
}

void Material::ClearTextures() {
    for (int i = 0; i < textures.Count(); i++) {
        if (textures[i]) {
            textures[i].Reset();
        }
    }
}

DataLayout& Material::GetLayout() {
    return shader->GetLayout();
}

u64 Material::GetHash() {
    // @TODO: dirty flag for the hash
    return TexturesShaderHash();
}

u64 Material::GetShaderHash() {
    return shader->GetHash();
}

u64 Material::GetTextureHash(int slot) {
    if (textures[slot]) {
        return textures[slot]->GetHash();
    }

    return 0;
}

u64 Material::GetTextureHashes() {
    u64 hashes[PD_MAX_TEXTURE_UNITS];

    for (int i = 0; i < textures.Count(); i++) {
        hashes[i] = GetTextureHash(i);
    }

    return DoHash(hashes, sizeof(hashes));
}

u64 Material::TexturesShaderHash() {
    u64 hashes[2];

    hashes[0] = GetTextureHashes();
    hashes[1] = GetShaderHash();

    return DoHash(hashes, sizeof(hashes));
}

Slice<Ref<Texture>> Material::GetTextures() {
    return textures;
}

}
