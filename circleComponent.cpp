#include "circleComponent.h"
#include "actor.h"

CircleComponent::CircleComponent(class Actor* owner)
	:Component(owner)
	,_radius(0.0f)
{

}

const Vector2& CircleComponent::GetCenter() const
{
	return _owner->GetPosition();
}

float CircleComponent::GetRadius() const
{
	
	return _owner->GetScale() * _radius;
	
}

/// <summary>
/// ÚG‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©
/// </summary>
bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
	Vector2 diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();

	float radiSq = a.GetRadius() + b.GetRadius();
	radiSq *= radiSq;

	return distSq <= radiSq;
}