#pragma once

#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Graphics/Renderer.h"
#include "Pandora/Graphics/ConstantBuffer.h"
#include "Pandora/Graphics/Model/Model.h"

namespace pd {

class ModelRenderer {
public:
    ModelRenderer();

    /**
     * \brief Updates the projection matrix.
     * 
     * \param viewProj The matrix.
     */
    void UpdateProjection(Mat4 viewProj);

    /**
     * \brief Draws the specified model.
     * 
     * \param model The model.
     */
    void Draw(Model& model);

private:
    Mat4 viewProj = Mat4(1.0f);

    Ref<Renderer> renderer;
    Ref<ConstantBuffer> mvpBuffer;
};

}
