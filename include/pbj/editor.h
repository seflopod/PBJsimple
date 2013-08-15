///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/editor.h
/// \author Benjamin Crist
///
/// \brief  pbj::Editor class header.

#ifndef PBJ_EDITOR_H_
#define PBJ_EDITOR_H_

#include "be/id.h"
#include "pbj/_pbj.h"
#include "pbj/window.h"
#include "pbj/engine.h"

#include "pbj/scene/scene.h"
#include "pbj/scene/ui_button.h"

#include <memory>

namespace pbj {

class Editor
{
public:
    Editor();
    ~Editor();

    void initUI();

    void run();

private:
    void onContextResized_(I32 width, I32 height);
    scene::UIButton* newButton_(const std::string& text,
                                const vec2& position,
                                const vec2& dimensions,
                                const std::function<void()>& callback,
                                scene::UIPanel* parent);

    Engine& engine_;
    Window& window_;

    scene::UIElement* last_created_focusable_element_;

    scene::UIRoot ui_;
    scene::UIPanel* menu_;
    scene::UILabel* frame_time_label_;

    std::unique_ptr<scene::Scene> scene_;

    Editor(const Editor&);
    void operator=(const Editor&);
};

} // namespace pbj

#endif
