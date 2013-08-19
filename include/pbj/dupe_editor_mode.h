///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/dupe_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::DupeEditorMode class header.

#ifndef PBJ_DUPE_EDITOR_MODE_H_
#define PBJ_DUPE_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class DupeEditorMode : public EditorMode
{
public:
    DupeEditorMode(Editor& editor);
    virtual ~DupeEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;

    scene::UIButton* dupe_btn_;
};

} // namespace pbj

#endif
