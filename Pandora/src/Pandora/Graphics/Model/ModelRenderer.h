#pragma once

#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Graphics/Renderer.h"
#include "Pandora/Graphics/ConstantBuffer.h"

#include "Pandora/Graphics/Model/Model.h"

namespace pd {

class ModelRenderer {
public:
    ModelRenderer();
    
    /// <summary>
    /// Updates the projection.
    /// Call this before drawing.
    /// </summary>
    /// <param name="mvp">The MVP matrix.</param>
    void UpdateProjection(Mat4 mvp);
    
    /// <summary>
    /// Draws the specified model.
    /// </summary>
    /// <param name="model">The model.</param>
    void Draw(Model& model);

private:
    Mat4 mvp = Mat4(1.0f);

    Ref<Renderer> renderer;
    Ref<ConstantBuffer> mvpBuffer;
};

}
