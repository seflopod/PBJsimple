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
#include "pbj/sw/resource_manager.h"

#include <memory>

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief Manages global engine objects.
///
/// \details Only one engine should be created per process.  Attempts to create
///        multiple engines will result in an exception.
class Engine
{
public:
    Engine(int*, char**);
    ~Engine();

    Window* getWindow() const;
    sw::ResourceManager& getResourceManager();

private:
    std::unique_ptr<Window> window_;
    sw::ResourceManager resource_mgr_;

    Engine(const Engine&);
    void operator=(const Engine&);
};

Engine& getEngine();

} // namespace pbj

#endif
