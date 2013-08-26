////////////////////////////////////////////////////////////////////////////////
/// \file   pbj\scene\bullet_component.h
///
/// \brief  Declares the bullet component class.
#ifndef PBJ_SCENE_BULLET_COMPONENT_H_
#define PBJ_SCENE_BULLET_COMPONENT_H_

#include "pbj/_pbj.h"

namespace pbj {
namespace scene {

class Entity;

////////////////////////////////////////////////////////////////////////////
/// \class  BulletComponent
///
/// \brief  Represents data and methods for an Entity that is used as a
///         bullet.  In this case it is used mostly for keeping track of the
///         Entity that shot the bullet.
///
/// \author Peter Bartosch
/// \date   2013-08-22
class BulletComponent
{
public:
    BulletComponent(Entity*);
    ~BulletComponent();

    void setShooter(Entity*);
    Entity* getShooter();

    Entity* getOwner() const;

private:
    Entity* _owner;
    Entity* _shooter;
};

} //namespace scene
} //namespace pbj

#endif
