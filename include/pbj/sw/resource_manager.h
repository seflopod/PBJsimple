///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/resource_manager.h
/// \author Benjamin Crist
///
/// \brief  pbj::sw::ResourceManager class header.

#ifndef PBJ_SW_RESOURCE_MANAGER_H_
#define PBJ_SW_RESOURCE_MANAGER_H_

#include "pbj/sw/resource_id.h"
#include "pbj/sw/sandwich.h"
#include "pbj/scene/ui_styles.h"
#include "pbj/_pbj.h"
#include "pbj/audio/buffer.h"
#include "pbj/gfx/material.h"

#include <unordered_map>
#include <memory>

namespace pbj {
namespace sw {

///////////////////////////////////////////////////////////////////////////////
/// \brief  A ResourceManager owns resource objects loaded from sandwiches.
///
/// \details When a request for a resource is made, the manager checks to see
///         if it has already been loaded and returns it if so, otherwise
///         it will try to load the resource from a sandwich and add it to
///         its collection of resources before returning it.
///
///         Any sandwiches that are loaded from will remain open, and any
///         resources loaded from them will remain valid, until the
///         ResourceManager is destroyed.
///
/// \author Ben Crist
class ResourceManager
{
public:
    ResourceManager();

    audio::Buffer* getSound(const ResourceId& id);
    const gfx::Material& getMaterial(const ResourceId& id);
    const gfx::TextureFont& getTextureFont(const ResourceId& id);
    const gfx::Texture& getTexture(const ResourceId& id);
    const scene::UIPanelStyle& getUIPanelStyle(const ResourceId& id);
    const scene::UIButtonStyle& getUIButtonStyle(const ResourceId& id);

private:
    Sandwich& getSandwich(const Id& sandwich_id);

    std::unordered_map<Id, std::shared_ptr<Sandwich> > sandwiches_;

    std::unordered_map<ResourceId, std::unique_ptr<audio::Buffer> > sounds_;
    std::unordered_map<ResourceId, std::unique_ptr<gfx::Material> > materials_;
    std::unordered_map<ResourceId, std::unique_ptr<gfx::TextureFont> > texture_fonts_;
    std::unordered_map<ResourceId, std::unique_ptr<gfx::Texture> > textures_;
    std::unordered_map<ResourceId, scene::UIPanelStyle> panel_styles_;
    std::unordered_map<ResourceId, scene::UIButtonStyle> button_styles_;

    ResourceManager(const ResourceManager&);
    void operator=(const ResourceManager&);
};

} // namespace pbj::sw
} // namespace pbj

#endif
