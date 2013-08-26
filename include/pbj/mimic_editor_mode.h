///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/mimic_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::MimicEditorMode class header.

#ifndef PBJ_MIMIC_EDITOR_MODE_H_
#define PBJ_MIMIC_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

////////////////////////////////////////////////////////////////////////////////
/// \brief  An editor mode class which allows the user to copy attributes from
///         one existing entity to another.
///
/// \details This mode is not yet fully implemented.  It would work like
///         DupeEditorMode except instead of creating new entities, left click
///         sets the closest entity to have the copied attributes.
///
/// \author Ben Crist
/// \date   2013-08-13
class MimicEditorMode : public EditorMode
{
public:
    MimicEditorMode(Editor& editor);
    virtual ~MimicEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;
};

} // namespace pbj

#endif
