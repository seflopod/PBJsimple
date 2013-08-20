///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/editor.h
/// \author Benjamin Crist
///
/// \brief  pbj::Editor class header.

#ifndef PBJ_EDITOR_H_
#define PBJ_EDITOR_H_

#include "pbj/editor_mode.h"
#include "pbj/engine.h"
#include "pbj/window.h"
#include "be/id.h"
#include "pbj/_pbj.h"
#include "pbj/scene/scene.h"
#include "pbj/scene/ui_button.h"

#include <memory>
#include <unordered_map>

namespace pbj {

class Editor
{
public:
    Editor();
    ~Editor();

    void initUI();
    void run(const std::string& sw_id, const std::string& map_id);

    scene::UIElement* getUIElement(const Id& id);

    EditorMode* getCurrentMode();
    void setMode(const Id& id);

    scene::Camera& getCamera() const;

private:
    void onContextResized_(I32 width, I32 height);
    scene::UIButton* newButton_(const Id& id,
                                const std::string& text,
                                const vec2& position,
                                const vec2& dimensions,
                                const std::function<void()>& callback,
                                scene::UIPanel* parent,
                                scene::UIElement*& previous_focusable);

    Engine& engine_;
    Window& window_;

    scene::UIRoot ui_;
    std::unordered_map<Id, scene::UIElement*> ui_elements_;
    scene::UIPanel* menu_;
    bool menu_toggled_;
    U32 menu_visible_counter_;
    scene::UILabel* frame_time_label_;

    std::unique_ptr<scene::Scene> scene_;

    std::unique_ptr<EditorMode> current_mode_;
    EditorMode* mouse_down_mode_[3];
    vec2 mouse_position_;

    Editor(const Editor&);
    void operator=(const Editor&);
};

} // namespace pbj

#endif
