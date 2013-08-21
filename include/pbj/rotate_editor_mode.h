///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/rotate_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::RotateEditorMode class header.

#ifndef PBJ_ROTATE_EDITOR_MODE_H_
#define PBJ_ROTATE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class RotateEditorMode : public EditorMode
{
public:
    RotateEditorMode(Editor& editor);
    virtual ~RotateEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;
};

} // namespace pbj

#endif
