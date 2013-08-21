
#ifdef PBJ_EDITOR

#include "pbj/editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

EditorMode::EditorMode(Editor& editor)
    : editor_(editor)
{
    for (U32 button = 0; button < 3; ++button)
    {
        btn_down_[button] = false;
        btn_down_moved_[button] = false;
    }
}

EditorMode::EditorMode(Editor& editor, scene::UIButton* btn)
    : editor_(editor),
      btn_(btn)
{
    for (U32 button = 0; button < 3; ++button)
    {
        btn_down_[button] = false;
        btn_down_moved_[button] = false;
    }
    if (btn_)
    {
        editor_.highlightUIButton(btn_);
    }
}

EditorMode::~EditorMode()
{
    if (btn_)
    {
        editor_.unhighlightUIButton(btn_);
    }
}

void EditorMode::onMouseDown(I32 button, const vec2& position)
{
    btn_down_pos_[button] = position;
    btn_down_[button] = true;
    btn_down_moved_[button] = false;
}

void EditorMode::onMouseUp(I32 button, const vec2& position)
{
    if (btn_down_[button])
    {
        if (btn_down_moved_[button] == false)
            onClick(button, btn_down_pos_[button], position);
        else
            onDragUpdate(button, btn_down_pos_[button], position);

        btn_down_[button] = false;
    }
}

void EditorMode::onMouseCancel(I32 button, const vec2& position)
{
    if (btn_down_[button])
    {
        if (btn_down_moved_[button])
            onDragUpdate(button, btn_down_pos_[button], btn_down_pos_[button]);

        btn_down_[button] = false;
    }
}

void EditorMode::onMouseMove(const vec2& position)
{
    for (U32 button = 0; button < 3; ++button)
    {
        if (btn_down_[button])
        {
            if (glm::length(position - btn_down_pos_[button]) > 1.0f)
            {
                btn_down_moved_[button] = true;
            }
            onDragUpdate(button, btn_down_pos_[button], position);
        }
    }
}

void EditorMode::onMouseWheel(I32 delta)
{
}

void EditorMode::onDragUpdate(I32 button, const vec2& start, const vec2& end)
{
}

void EditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
}

} // namespace pbj

#endif
