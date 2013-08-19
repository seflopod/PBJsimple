///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::Editor class header.

#ifndef PBJ_EDITOR_MODE_H_
#define PBJ_EDITOR_MODE_H_

#include "pbj/_pbj.h"
#include "be/id.h"

#include "scene/ui_button.h"

namespace pbj {

class Editor;

class EditorOperation
{
public:
    virtual ~EditorOperation() {}
};

class EditorMode
{
public:
    EditorMode(Editor& editor) : editor_(editor) {}
    EditorMode(Editor& editor, scene::UIButton* btn) : editor_(editor), btn_(btn)
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

    virtual ~EditorMode()
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

    virtual const Id& getId() const = 0;

protected:
    Editor& editor_;

    scene::UIButton* btn_;
};

} // namespace pbj

#endif
