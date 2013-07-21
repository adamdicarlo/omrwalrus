#ifndef __KT_ENTITY_INCLUDED__
#define __KT_ENTITY_INCLUDED__

#include "Box2D/Body.h"
#include "basictypes.hpp"
#include <OpenLayer.hpp>

namespace kt {

class GameWorld;
using namespace ol;

enum EntityType {
	EntityBall,
	EntityBukkit,
	EntityCat,
	EntityMat,
 	EntityPusher,
	EntityWalrus
};

class Entity {
public:
	Entity(GameWorld* pw) : m_pWorld(pw) { }
	virtual ~Entity() {}
	virtual void Init() = 0;
	virtual bool Tick(timestamp) = 0;
	virtual void Render() = 0;
	virtual void SetPos(Vec2D v) {
		m_body.position.x = v.x;
		m_body.position.y = v.y;
	}
	virtual void SetVel(Vec2D v) {
		m_body.velocity.x = v.x;
		m_body.velocity.y = v.y;
	}
	virtual void Struck(counted_ptr<Entity>&) { }
	
	EntityType m_type;
	GameWorld* m_pWorld;
	Body       m_body;
};

typedef counted_ptr<Entity>		EntityPtr;
typedef std::list<EntityPtr>	EntityList;


} // namespace kt


#endif // __KT_ENTITY_INCLUDED__

