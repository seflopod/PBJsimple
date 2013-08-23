///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_image.h
/// \author Josh Douglas
///
/// \brief  pbj::scene::UIImage class header.

#ifndef PBJ_SCENE_UI_IMAGE_H_
#define PBJ_SCENE_UI_IMAGE_H_

#include "pbj/scene/ui_element.h"
#include "pbj/gfx/texture.h"

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Text Image UI element.
class UIImage : public UIElement
{
public:
    UIImage();
    virtual ~UIImage();
    void setTexture(const gfx::Texture* texture);
    const gfx::Texture* getTexture() const;

private:
    gfx::Texture* texture_;
};

} // namespace pbj::scene
} // namespace pbj

#endif
