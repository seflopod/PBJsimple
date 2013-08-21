///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/add_clobber_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::AddEditorMode class header.

#ifndef PBJ_ADD_CLOBBER_EDITOR_MODE_H_
#define PBJ_ADD_CLOBBER_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

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
