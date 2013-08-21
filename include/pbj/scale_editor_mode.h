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
