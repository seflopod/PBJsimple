///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/decorate_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::DecorateEditorMode class header.

#ifndef PBJ_DECORATE_EDITOR_MODE_H_
#define PBJ_DECORATE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

////////////////////////////////////////////////////////////////////////////////
/// \brief  An editor mode class which allows the user to change the material
///         and/or type of existing entities.
///
/// \details Works exactly like the left click (add) functionality of
///         AddClobberEditorMode except instead of adding a new entity, the
///         closest existing entity is changed.
///
/// \author Ben Crist
/// \date   2013-08-13
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
