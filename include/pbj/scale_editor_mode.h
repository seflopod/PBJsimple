///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scale_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::ScaleEditorMode class header.

#ifndef PBJ_SCALE_EDITOR_MODE_H_
#define PBJ_SCALE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"
#include "pbj/scene/entity.h"

namespace pbj {

////////////////////////////////////////////////////////////////////////////////
/// \brief  An editor mode class which allows the user to change the scale of
///         an existing entity in the scene.
///
/// \details Right and left mouse button dragging both have the same effect.
///         Dragging left or right decreases or increases (respectively) the
///         entity's scale on it's local x-axis.  Dragging up and down
///         increases or decreases (respectively) its scale on the local y-axis.
///
/// \author Ben Crist
/// \date   2013-08-13
class ScaleEditorMode : public EditorMode
{
public:
    ScaleEditorMode(Editor& editor);
    virtual ~ScaleEditorMode();

    virtual const Id& getId() const;

    virtual void onMouseDown(I32 button, const vec2& position);
    virtual void onMouseUp(I32 button, const vec2& position);
    virtual void onMouseCancel(I32 button, const vec2& position);
    virtual void onDragUpdate(I32 button, const vec2& start, const vec2& end);

private:
    static Id id_;

    scene::Entity* active_entity_;
    vec2 original_scale_;
};

} // namespace pbj

#endif
