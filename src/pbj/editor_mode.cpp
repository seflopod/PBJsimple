
#ifdef PBJ_EDITOR

#include "pbj/editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

EditorMode::EditorMode(Editor& editor)
    : editor_(editor)
{
}

EditorMode::EditorMode(Editor& editor, scene::UIButton* btn)
    : editor_(editor),
      btn_(btn)
{
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
}

void EditorMode::onMouseUp(I32 button, const vec2& position)
{
}

void EditorMode::onMouseCancel(I32 button, const vec2& position)
{
}

void EditorMode::onMouseMove(I32 button, const vec2& position)
{
}


} // namespace pbj

#endif
