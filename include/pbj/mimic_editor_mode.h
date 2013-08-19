///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/mimic_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::MimicEditorMode class header.

#ifndef PBJ_MIMIC_EDITOR_MODE_H_
#define PBJ_MIMIC_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class MimicEditorMode : public EditorMode
{
public:
    MimicEditorMode(Editor& editor);
    virtual ~MimicEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;

    scene::UIButton* mimic_btn_;
};

} // namespace pbj

#endif
