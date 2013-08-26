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

////////////////////////////////////////////////////////////////////////////////
/// \brief  An editor mode determines how user input events affect the scene.
///
/// \details In addition, the construction and destruction of editor modes
///         corresponds with entering and exiting that mode, so operations
///         like toggling panel visibility and changing button appearances
///         can be done in constructors/destructors.  When the editor mode
///         changes, the previous editor mode will be destroyed before the
///         new mode's constructor is called.
///
/// \author Ben Crist
/// \date   2013-08-13
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
    
    vec2 btn_down_pos_[3];      ///< The mouse position when each of the first 3 buttons was pressed.
    bool btn_down_[3];          ///< Whether or not each of the first 3 buttons (L,R,M) was pressed while in this mode.
    bool btn_down_moved_[3];    ///< Whether or not the mouse has moved since the mouse button was pressed.
};

} // namespace pbj

#endif
