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
    virtual void onMouseMove(I32 button, const vec2& position);
    
protected:
    Editor& editor_;

    scene::UIButton* btn_;
};

} // namespace pbj

#endif
