////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\include\pbj\scene\bullet_component.h
///
/// \brief  Declares the bullet component class.
////////////////////////////////////////////////////////////////////////////////
#ifndef BULLET_COMPONENT_H_
#define BULLET_COMPONENT_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////
    /// \class  BulletComponent
    ///
    /// \brief  Represents data and methods for an Entity that is used as a
    ///         bullet.  In this case it is used mostly for keeping track of the
    ///         Entity that shot the bullet.
    ///
    /// \author Peter Bartosch
    /// \date   2013-08-22
    ////////////////////////////////////////////////////////////////////////////
    class BulletComponent
    {
    public:
        BulletComponent(void*);
        ~BulletComponent();

        void setShooter(void*);
        void* getShooter();

        void* getOwner() const;

    private:
        void* _owner;
        void* _shooter;
    };
} //namespace scene
} //namespace pbj
#endif