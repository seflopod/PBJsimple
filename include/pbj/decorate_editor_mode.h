///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/decorate_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::DecorateEditorMode class header.

#ifndef PBJ_DECORATE_EDITOR_MODE_H_
#define PBJ_DECORATE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class DecorateEditorMode : public EditorMode
{
public:
    DecorateEditorMode(Editor& editor);
    virtual ~DecorateEditorMode();

    virtual const Id& getId() const;

    virtual void onClick(I32 button, const vec2& start, const vec2& end);

private:
    static Id id_;
};

} // namespace pbj

#endif
