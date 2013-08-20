///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/look_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::LookEditorMode class header.

#ifndef PBJ_LOOK_EDITOR_MODE_H_
#define PBJ_LOOK_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class LookEditorMode : public EditorMode
{
public:
    LookEditorMode(Editor& editor);
    virtual ~LookEditorMode();

    virtual const Id& getId() const;
    
    virtual void onMouseDown(I32 button, const vec2& position);
    virtual void onDragUpdate(I32 button, const vec2& start, const vec2& end);
    virtual void onClick(I32 button, const vec2& start, const vec2& end);

private:
    static Id id_;

    vec2 drag_start_camera_pos_;
};

} // namespace pbj

#endif
