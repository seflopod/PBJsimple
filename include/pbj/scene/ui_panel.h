///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_panel.h
/// \author Ben Crist

#ifndef PBJ_SCENE_UI_PANEL_H_
#define PBJ_SCENE_UI_PANEL_H_

#include "pbj/scene/ui_element.h"

#include "pbj/engine.h"

#include <memory>
#include <vector>

namespace pbj {
namespace scene {

class UIRoot;

class UIPanel : public UIElement
{
    friend class UIRoot;

public:
    UIPanel();
    virtual ~UIPanel();

    void setStyle(const sw::ResourceId& panel_style);
    const sw::ResourceId& getStyle() const;

    void setScale(const vec2& scale);
    const vec2& getScale() const;

    void addElement(std::unique_ptr<UIElement>&& element);
    
    virtual UIElement* getElementAt(const ivec2& screen_position);

    virtual void draw();

private:
    virtual void onBoundsChange_();

    void calculateTransform_();

    std::vector<std::unique_ptr<UIElement> > elements_;

    vec2 scale_;

    sw::ResourceId style_id_;
    const UIPanelStyle* style_;

    vec2 border_bounds_[4];
    mat4 panel_transform_;
    bool panel_transform_valid_;

    mat4 view_matrix_;
    mat4 inv_view_matrix_;

    UIPanel(const UIPanel&);
    void operator=(const UIPanel&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
