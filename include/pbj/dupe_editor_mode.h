///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/dupe_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::DupeEditorMode class header.

#ifndef PBJ_DUPE_EDITOR_MODE_H_
#define PBJ_DUPE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {
;
////////////////////////////////////////////////////////////////////////////////
/// \brief  An editor mode class which allows the user to duplicate existing
///         entities.
///
/// \details This mode is not yet fully implemented.  Right clicking would
///         result in the closest entity's attributes (type, material, scale,
///         rotation).  Left clicking would create a new entity using the
///         copied attributes.
///
/// \author Ben Crist
/// \date   2013-08-13
class DupeEditorMode : public EditorMode
{
public:
    DupeEditorMode(Editor& editor);
    virtual ~DupeEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;
};

} // namespace pbj

#endif
