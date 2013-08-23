#include "pbj/scene/ai_component.h"

#include "pbj/scene/entity.h"
#include "pbj/game.h"

//I just hate seeing std:: all the damn time
using std::sin;
using std::cos;

namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \fn AIComponent::AIComponent(Entity* owner)
///
/// \brief  Constructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in,out] owner   If non-null, the owner.
////////////////////////////////////////////////////////////////////////////////
AIComponent::AIComponent(Entity* owner) :
                _seePlayer(false),
                _sweepCnt(0),
                _moveStart(0),
                _dir(0)
{
    assert((Entity*)owner);
    _owner = owner;
    srand(time(nullptr));
}

////////////////////////////////////////////////////////////////////////////////
/// \fn AIComponent::~AIComponent()
///
/// \brief  Destructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
AIComponent::~AIComponent()
{
    _owner = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AIComponent::update(F32 dt)
///
/// \brief  Updates the given dt.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  dt  The dt.
///
/// \details This is where the bad magic happens for the AI.  Rather than keep
///          track of the position of each other PlayerComponent, the AI sweeps
///          (6 deg/frame) a ray to find possible targets.  If a target is
///          found, three shots are fired at it.  I don't think that works as
///          intended, based on how shooting works for a PlayerComponent.  The
///          raycast is supposed to get the closest Entity, which should mean
///          that any terrain that is in the way should prevent the AI from
///          seeing a PlayerComponent.  This is not working.  Anyway, once
///          shooting stuff is done the AI moves.  It randomly picks a direction
///          to move in for a certain amount of time.  While moving it has a one
///          in three chance of jumping or using it's thrusters (each, so it
///          effectively has a one in three chance of only moving left or
///          right).
////////////////////////////////////////////////////////////////////////////////
void AIComponent::update(F32 dt)
{
    F32 castDistance = 100.0f;

    Transform& xf = _owner->getTransform();
    PlayerComponent* player = _owner->getPlayerComponent();

    const vec2& scale = xf.getScale();

    F32 r1 = (scale.x>scale.y) ? scale.x : scale.y;
    F32 r2 = r1 + castDistance;
    F32 angle = DEGTORAD * (6 * _sweepCnt);

    //make the starting point on a circle off of the body, this way it isn't
    //counted in the raycast.

    Game::instance()->currentScene().getWorld()->RayCast(this,
                                    b2Vec2(r1*cos(angle),r1*sin(angle)), //start
                                    b2Vec2(r2*cos(angle),r2*sin(angle))); //end
    if(_seePlayer)
    {
        player->fire(_target.x, _target.y);
        player->fire(_target.x, _target.y);
        player->fire(_target.x, _target.y);
        _seePlayer = false;
    }

    //Do movement
    if(_moveStart == 0 || _moveStart == _sweepCnt)
    {
        _dir = rand()%2;
        _moveStart = rand()%60;
    }

    if(_dir)
        player->moveLeft();
    else
        player->moveRight();

    I32 jumpThrust = rand()%3;
    switch(jumpThrust)
    {
    case 0: player->jump(); break;
    case 1: player->doThrust(); break;
    default: break;
    }

    if(++_sweepCnt == 60)
        _sweepCnt = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn float32 AIComponent::ReportFixture(b2Fixture* fixture,
///     const b2Vec2& point, const b2Vec2& normal, float32 fraction)
///
/// \brief  Implementation of the ReportFixture method for b2RayCastCallback
///         class
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in] fixture     The fixture hit by the raycaset
/// \param  point           The point of intersection.
/// \param  normal          The normal at the point of intersection.
/// \param  fraction        The fraction.
///
/// \return The passed fraction because we only want the closest fixture.
///
/// \details    This is meant to deal with whatever is detected on a raycast.
///             For our purposes this means we flag that a player has been
///             seen if the reported fixture is attachted to a PlayerComponent.
///             If that is the case, we set it to be our target.
////////////////////////////////////////////////////////////////////////////////
float32 AIComponent::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
    Entity* e = (Entity*)(fixture->GetBody()->GetUserData());
    _seePlayer = e->getType()==Entity::EntityType::Player;
    if(_seePlayer)
        _target = e->getTransform().getPosition();

    return fraction; //because we only want the closest fixture;
}

} // namespace pbj::scene
} // namespace pbj
