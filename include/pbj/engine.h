///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/engine.h
/// \author Benjamin Crist
///
/// \brief  pbj::Engine class header.

#ifndef PBJ_ENGINE_H_
#define PBJ_ENGINE_H_

#include "be/id.h"
#include "pbj/_pbj.h"
#include "pbj/window.h"

#include <memory>

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief Manages global engine objects.
/// \details Only one engine should be created per process.  Attempts to create
///        multiple engines will result in an exception.
class Engine
{
public:
   Engine();
   ~Engine();

   Window* getWindow() const;

private:
    std::unique_ptr<Window> window_;

    Engine(const Engine&);
    void operator=(const Engine&);
};

Engine& getEngine();

} // namespace pbj

#endif
