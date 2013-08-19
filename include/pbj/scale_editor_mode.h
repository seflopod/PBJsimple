///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scale_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::ScaleEditorMode class header.

#ifndef PBJ_SCALE_EDITOR_MODE_H_
#define PBJ_SCALE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class ScaleEditorMode : public EditorMode
{
public:
    ScaleEditorMode(Editor& editor);
    virtual ~ScaleEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;

    
};

} // namespace pbj

#endif
