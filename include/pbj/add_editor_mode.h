///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/add_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::AddEditorMode class header.

#ifndef PBJ_ADD_EDITOR_MODE_H_
#define PBJ_ADD_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class AddEditorMode : public EditorMode
{
public:
    AddEditorMode(Editor& editor);
    virtual ~AddEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;

    scene::UIButton* add_btn_;
};

} // namespace pbj

#endif
