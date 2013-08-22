////////////////////////////////////////////////////////////////////////////////
/// \file   pbj/transform.h
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-07-18
/// \brief  A class for representing the transformation of an object.
/// \details This class is mostly a wrapper for glm calls.  It stores the
///         position, rotation, and scale of an object.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_TRANSFORM_H_
#define PBJ_TRANSFORM_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

    //Forward declaration
    class Entity;

    ////////////////////////////////////////////////////////////////////////////
    /// \class  Transform
    ///
    /// \brief  A class to represent position data for an Entity.
    ///
    /// \author Peter Bartosch
    /// \date   2013-08-22
    ////////////////////////////////////////////////////////////////////////////
    class Transform
    {
    public:
        Transform(Entity*);
        ~Transform();

        void rotate(F32);

        void move(F32, F32);
        void move(const vec2&);

        const vec2& getPosition() const;
        void setPosition(F32, F32);
        void setPosition(const vec2&);

        F32 getRotation() const;
        void setRotation(F32 rotation);

        const vec2& getScale() const;
        void setScale(F32, F32);
        void setScale(const vec2&);

        scene::Entity* getOwner();

        void updateOwnerRigidbody();
    private:
        Entity* _owner;
        vec2 _position;
        F32 _rotation;
        vec2 _scale;
    };

} // namespace pbj::scene
} // namespace pbj

#endif
