///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/add_clobber_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::AddEditorMode class header.

#ifndef PBJ_ADD_CLOBBER_EDITOR_MODE_H_
#define PBJ_ADD_CLOBBER_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

////////////////////////////////////////////////////////////////////////////////
/// \brief  An editor mode class which allows the user to add or remove
///         (clobber) entities in the scene.
///
/// \details Right clicking removes the nearest entity to the cursor (within a
///         reasonable distance).  Left clicking adds a new entity at the
///         cursor's position.  The type of entity to add is determined by the
///         current selection made in the editor's material UI panel.
///
/// \author Ben Crist
/// \date   2013-08-13
class AddClobberEditorMode : public EditorMode
{
public:
    AddClobberEditorMode(Editor& editor);
    virtual ~AddClobberEditorMode();

    virtual const Id& getId() const;

      virtual void onClick(I32 button, const vec2& start, const vec2& end);

private:
    static Id id_;
};

} // namespace pbj

#endif
