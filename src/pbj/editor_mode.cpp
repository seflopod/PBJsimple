
#ifdef PBJ_EDITOR

#include "pbj/editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

EditorMode::EditorMode(Editor& editor)
    : editor_(editor)
{
    for (U32 button = 0; button < 3; ++button)
    {
        btn_down_[button] = false;
        btn_down_moved_[button] = false;
    }
}

EditorMode::EditorMode(Editor& editor, scene::UIButton* btn)
    : editor_(editor),
      btn_(btn)
{
    for (U32 button = 0; button < 3; ++button)
    {
        btn_down_[button] = false;
        btn_down_moved_[button] = false;
    }
    if (btn_)
    {
        btn_->setStyle(scene::UIButton::SNormal,         sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.normal")));
        btn_->setStyle(scene::UIButton::SHovered,        sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.hovered")));
        btn_->setStyle(scene::UIButton::SActive,         sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.active")));
        btn_->setStyle(scene::UIButton::SFocused,        sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.focused")));
        btn_->setStyle(scene::UIButton::SFocusedHovered, sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.focused_hovered")));
        btn_->setStyle(scene::UIButton::SFocusedActive,  sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.focused_active")));
        btn_->setStyle(scene::UIButton::SDisabled,       sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.disabled")));
    }
}

EditorMode::~EditorMode()
{
    if (btn_)
    {
        btn_->setStyle(scene::UIButton::SNormal,         sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.normal")));
        btn_->setStyle(scene::UIButton::SHovered,        sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.hovered")));
        btn_->setStyle(scene::UIButton::SActive,         sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.active")));
        btn_->setStyle(scene::UIButton::SFocused,        sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused")));
        btn_->setStyle(scene::UIButton::SFocusedHovered, sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused_hovered")));
        btn_->setStyle(scene::UIButton::SFocusedActive,  sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused_active")));
        btn_->setStyle(scene::UIButton::SDisabled,       sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.disabled")));
    }
}

void EditorMode::onMouseDown(I32 button, const vec2& position)
{
    btn_down_pos_[button] = position;
    btn_down_[button] = true;
    btn_down_moved_[button] = false;
}

void EditorMode::onMouseUp(I32 button, const vec2& position)
{
    if (btn_down_[button])
    {
        if (btn_down_moved_[button] == false)
            onClick(button, btn_down_pos_[button], position);
        else
            onDragUpdate(button, btn_down_pos_[button], position);

        btn_down_[button] = false;
    }
}

void EditorMode::onMouseCancel(I32 button, const vec2& position)
{
    if (btn_down_[button])
    {
        if (btn_down_moved_[button])
            onDragUpdate(button, btn_down_pos_[button], btn_down_pos_[button]);

        btn_down_[button] = false;
    }
}

void EditorMode::onMouseMove(const vec2& position)
{
    for (U32 button = 0; button < 3; ++button)
    {
        if (btn_down_[button])
        {
            btn_down_moved_[button] = true;
            onDragUpdate(button, btn_down_pos_[button], position);
        }
    }
}

void EditorMode::onDragUpdate(I32 button, const vec2& start, const vec2& end)
{
}

void EditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
}

} // namespace pbj

#endif
