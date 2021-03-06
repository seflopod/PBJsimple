///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/move_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::MoveEditorMode class header.

#ifndef PBJ_MOVE_EDITOR_MODE_H_
#define PBJ_MOVE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"
#include "pbj/scene/entity.h"

namespace pbj {

////////////////////////////////////////////////////////////////////////////////
/// \brief  An editor mode class which allows the user to move an existing
///         entity in the scene.
///
/// \details Right and left mouse button dragging both have the same effect.
///
/// \author Ben Crist
/// \date   2013-08-13
class MoveEditorMode : public EditorMode
{
public:
    MoveEditorMode(Editor& editor);
    virtual ~MoveEditorMode();

    virtual const Id& getId() const;

    virtual void onMouseDown(I32 button, const vec2& position);
    virtual void onMouseUp(I32 button, const vec2& position);
    virtual void onMouseCancel(I32 button, const vec2& position);
    virtual void onDragUpdate(I32 button, const vec2& start, const vec2& end);

private:
    static Id id_;

    scene::Entity* moving_entity_;
    vec2 original_position_;
};

} // namespace pbj

#endif
