////////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/material.cpp
///
/// \brief  Implements the Material class.
#include "pbj\gfx\material.h"

#include "pbj/sw/resource_manager.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a texture from a sandwich.
/// \param  1 The id of the texture.
#define PBJ_GFX_MATERIAL_SQL_LOAD "SELECT color, texture_id, texture_mode " \
            "FROM sw_materials WHERE id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_GFX_MATERIAL_SQLID_LOAD PBJ_GFX_MATERIAL_SQL_LOAD
#else
// TODO: precalculate ids.
#define PBJ_GFX_MATERIAL_SQLID_LOAD PBJ_GFX_MATERIAL_SQL_LOAD
#endif

namespace pbj {
namespace gfx {

////////////////////////////////////////////////////////////////////////////////
/// \brief  Default Material constructor
///
/// \author Josh Douglas
/// \date   2013-08-13
Material::Material(const sw::ResourceId& id, const color4& color, const Texture* texture, GLenum texture_mode)
    : id_(id),
      color_(color),
      tex_(texture),
      tex_mode_(texture_mode)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Default Material destructor
///
/// \author Josh Douglas
/// \date   2013-08-13
Material::~Material()
{
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  returns the material's color.
///
/// \author Josh Douglas
/// \date   2013-08-13
const color4& Material::getColor() const
{
    return color_;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Get function to return a pointer to this material's texture
///
/// \author Josh Douglas
/// \date   2013-08-13
const Texture* Material::getTexture() const
{
    return tex_;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the texturemode.
///
/// \author Josh Douglas
/// \date   2013-08-13
GLenum Material::getTextureMode() const
{
    return tex_mode_;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Ensures that the current vertex color is set to this material's
///         color and that the proper texture settings are enabled so that
///         geometry appears to have this material's apperance.
///
/// \author Ben Crist
void Material::use() const
{
    if (tex_)
        tex_->enable(tex_mode_);
    else
        Texture::disable();

    glColor4fv(glm::value_ptr(color_));
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the ResourceId which describes where this material can
///         be loaded from in a sandwich.
const sw::ResourceId& Material::getId() const
{
    return id_;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Loads the material having the requested Id from the sandwich
///         provided.
///
/// \details If the material has a texture, it will be retrieved using the
///         ResourceManager provided.
///
/// \param  sandwich the Sandwich to load from.
/// \param  id The Id of the material in the sandwich to load.
/// \param  rm The resourceManager to retrieve the material's texture from, if
///         necessary.
/// \return A unique_ptr to the material, or an empty unique_ptr if there is a
///         problem loading it.
std::unique_ptr<Material> loadMaterial(sw::Sandwich& sandwich, const Id& id, sw::ResourceManager& rm)
{
    std::unique_ptr<Material> result;

    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt stmt = cache.hold(Id(PBJ_GFX_MATERIAL_SQLID_LOAD), PBJ_GFX_MATERIAL_SQL_LOAD);

        stmt.bind(1, id.value());
        if (stmt.step())
        {
            color4 color(stmt.getColor(0));

            const Texture* tex(nullptr);
            if (stmt.getType(1) != SQLITE_NULL)
            {
                Id texture_id(stmt.getUInt64(1));
                tex = &rm.getTexture(sw::ResourceId(sandwich.getId(), texture_id));
            }

            GLenum texture_mode;
            switch (stmt.getUInt(2))
            {
                case 0:     texture_mode = GL_MODULATE; break;
                case 1:     texture_mode = GL_DECAL; break;
                case 2:     texture_mode = GL_ADD; break;
                default:    texture_mode = GL_REPLACE; break;
            }

            result.reset(new Material(sw::ResourceId(sandwich.getId(), id), color, tex, texture_mode));
        }
        else
            throw std::runtime_error("Material not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading material!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "Material ID: " << id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::exception& err)
   {
      PBJ_LOG(VWarning) << "Exception while loading material!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "Material ID: " << id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
   }

   return result;
}

} // namespace gfx
} // namespace pbj
