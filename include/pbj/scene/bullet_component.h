#ifndef BULLET_COMPONENT_H_
#define BULLET_COMPONENT_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace scene
{
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