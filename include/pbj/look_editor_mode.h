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

private:
    static Id id_;

    scene::UIButton* look_btn_;
};

} // namespace pbj

#endif
