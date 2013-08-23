///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/resource_manager.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::sw::ResourceManager functions.

#include "pbj/sw/resource_manager.h"

#include "pbj/gfx/material.h"
#include "pbj/gfx/texture_font.h"
#include "pbj/gfx/texture.h"
#include "pbj/audio/buffer.h"
#include "pbj/sw/sandwich_open.h"
#include "pbj/_gl.h"

#include <iostream>

namespace pbj {
namespace sw {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs an empty ResourceManager.
ResourceManager::ResourceManager()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys this ResourceManager, along with all resources owned by
///         the manager.
ResourceManager::~ResourceManager()
{
}

///////////////////////////////////////////////////////////////////////////////
audio::Buffer* ResourceManager::getSound(const ResourceId& id)
{
    auto i = sounds_.find(id);
    if (i != sounds_.end())
        return i->second.get();

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<audio::Buffer> ptr = audio::loadSound(sandwich, id.resource);

    if (ptr)
    {
        audio::Buffer* ab = ptr.get();
        sounds_[id] = std::move(ptr);
        return sounds_[id].get();
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "Sound not found!" << PBJ_LOG_NL
                    << "Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                    << "   Sound ID: " << id.resource << PBJ_LOG_END;

    throw std::invalid_argument("Sound not found!");
}

///////////////////////////////////////////////////////////////////////////////
const gfx::Material& ResourceManager::getMaterial(const ResourceId& id)
{
    auto i = materials_.find(id);
    if (i != materials_.end())
        return *i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<gfx::Material> ptr = gfx::loadMaterial(sandwich, id.resource, *this);

    if (ptr)
    {
        gfx::Material* mat = ptr.get();
        materials_[id] = std::move(ptr);
        return *mat;
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "Material not found!" << PBJ_LOG_NL
                    << "   Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                    << "TextureFont ID: " << id.resource << PBJ_LOG_END;

    throw std::invalid_argument("TextureFont not found!");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a TextureFont from this ResourceManager, loading it from
///         a sandwich if necessary.
///
/// \param  id The ResourceId of the asset to retrieve.
/// \return A reference to the requested resource.
/// \throws std::invalid_argument If the TextureFont could not be loaded.
const gfx::TextureFont& ResourceManager::getTextureFont(const ResourceId& id)
{
    auto i = texture_fonts_.find(id);
    if (i != texture_fonts_.end())
        return *i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<gfx::TextureFont> ptr = gfx::loadTextureFont(sandwich, id.resource, *this);

    if (ptr)
    {
        gfx::TextureFont* font = ptr.get();
        texture_fonts_[id] = std::move(ptr);
        return *font;
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "TextureFont not found!" << PBJ_LOG_NL
                    << "   Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                    << "TextureFont ID: " << id.resource << PBJ_LOG_END;

    throw std::invalid_argument("TextureFont not found!");
}
    
///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a Texture from this ResourceManager, loading it from a
///         sandwich if necessary.
///
/// \param  id The ResourceId of the texture to retrieve.
/// \return A reference to the requested resource.
/// \throws std::invalid_argument If the Texture could not be loaded.
const gfx::Texture& ResourceManager::getTexture(const ResourceId& id)
{
    auto i = textures_.find(id);
    if (i != textures_.end())
        return *i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<gfx::Texture> ptr = gfx::loadTexture(sandwich, id.resource);

    if (ptr)
    {
        gfx::Texture* tex = ptr.get();
        textures_[id] = std::move(ptr);
        return *tex;
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "Texture not found!" << PBJ_LOG_NL
                    << "Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                    << " Texture ID: " << id.resource << PBJ_LOG_END;

    throw std::invalid_argument("Texture not found!");
}

///////////////////////////////////////////////////////////////////////////////
const scene::UIPanelStyle& ResourceManager::getUIPanelStyle(const ResourceId& id)
{
    auto i = panel_styles_.find(id);
    if (i != panel_styles_.end())
        return i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    return panel_styles_[id] = scene::loadUIPanelStyle(sandwich, id.resource);
}

///////////////////////////////////////////////////////////////////////////////
const scene::UIButtonStyle& ResourceManager::getUIButtonStyle(const ResourceId& id)
{
    auto i = button_styles_.find(id);
    if (i != button_styles_.end())
        return i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    return button_styles_[id] = scene::loadUIButtonStyle(sandwich, id.resource, *this);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Opens a sandwich if it is not yet in use by this ResourceManager,
///         or returns an existing sandwich if already in use.
///
/// \param  sandwich_id The Id of the sandwich to open.
/// \return A reference to the sandwich.
/// \throws std::invalid_argument If the sandwich can't be opened.
Sandwich& ResourceManager::getSandwich(const Id& sandwich_id)
{
    auto i = sandwiches_.find(sandwich_id);
    if (i != sandwiches_.end())
        return *i->second;

    // if we get to here, the sandwich is not open yet.
    std::shared_ptr<Sandwich> sw = open(sandwich_id);

    Sandwich* ptr = sw.get();

    if (ptr)
    {
        sandwiches_[sandwich_id] = sw;
    }
    else
    {
        throw std::invalid_argument("The sandwich could not be opened!");
    }

    return *ptr;
}

} // namespace pbj::gfx
} // namespace pbj
