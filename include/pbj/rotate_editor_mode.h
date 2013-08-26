///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/rotate_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::RotateEditorMode class header.

#ifndef PBJ_ROTATE_EDITOR_MODE_H_
#define PBJ_ROTATE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"
#include "pbj/scene/entity.h"

namespace pbj {

////////////////////////////////////////////////////////////////////////////////
/// \brief  An editor mode class which allows the user to rotate an existing
///         entity in the scene.
///
/// \details Right and left mouse button dragging both have the same effect.
///         Drag to the left or up to rotate CCW or right or down to rotate
///         CW.
///
/// \author Ben Crist
/// \date   2013-08-13
class RotateEditorMode : public EditorMode
{
public:
    RotateEditorMode(Editor& editor);
    virtual ~RotateEditorMode();

    virtual const Id& getId() const;

    virtual void onMouseDown(I32 button, const vec2& position);
    virtual void onMouseUp(I32 button, const vec2& position);
    virtual void onMouseCancel(I32 button, const vec2& position);
    virtual void onDragUpdate(I32 button, const vec2& start, const vec2& end);

private:
    static Id id_;

    scene::Entity* active_entity_;
    F32 original_rotation_;
};

} // namespace pbj

#endif
