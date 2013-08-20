///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/clobber_editor_mode.h
/// \author Benjamin Crist
///
/// \brief  pbj::ClobberEditorMode class header.

#ifndef PBJ_CLOBBER_EDITOR_MODE_H_
#define PBJ_CLOBBER_EDITOR_MODE_H_

#include "pbj/editor_mode.h"

namespace pbj {

class ClobberEditorMode : public EditorMode
{
public:
    ClobberEditorMode(Editor& editor);
    virtual ~ClobberEditorMode();

    virtual const Id& getId() const;

private:
    static Id id_;
};

} // namespace pbj

#endif
