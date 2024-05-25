#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, _angularSpeed(0.0f)
	, _forwardSpeed(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(_angularSpeed))
	{
		float rot = _owner->GetRotation();
		rot += _angularSpeed * deltaTime;
		_owner->SetRotation(rot);
	}

	if (!Math::NearZero(_forwardSpeed))
	{
		Vector2 pos = _owner->GetPosition();
		pos += _owner->GetForward() * _forwardSpeed * deltaTime;

		// (Screen wrapping code only for asteroids)
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

		_owner->SetPosition(pos);
	}
}