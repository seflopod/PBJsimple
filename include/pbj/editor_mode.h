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
    EditorMode(Editor& editor);
    EditorMode(Editor& editor, scene::UIButton* btn);

    virtual ~EditorMode();

    virtual const Id& getId() const = 0;
    
    virtual void onMouseDown(I32 button, const vec2& position);
    virtual void onMouseUp(I32 button, const vec2& position);
    virtual void onMouseCancel(I32 button, const vec2& position);
    virtual void onMouseMove(const vec2& position);
    virtual void onMouseWheel(I32 delta);

    virtual void onDragUpdate(I32 button, const vec2& start, const vec2& end);
    virtual void onClick(I32 button, const vec2& start, const vec2& end);
    
protected:
    Editor& editor_;

    scene::UIButton* btn_;
    
    vec2 btn_down_pos_[3];
    bool btn_down_[3];
    bool btn_down_moved_[3];
};

} // namespace pbj

#endif
