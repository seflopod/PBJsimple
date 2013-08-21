///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/move_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::MoveEditorMode class header.

#ifndef PBJ_MOVE_EDITOR_MODE_H_
#define PBJ_MOVE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class MoveEditorMode : public EditorMode
{
public:
    MoveEditorMode(Editor& editor);
    virtual ~MoveEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;
};

} // namespace pbj

#endif
