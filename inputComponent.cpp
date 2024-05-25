#include "inputComponent.h"
#include "actor.h"

InputComponent::InputComponent(class Actor* owner)
	:MoveComponent(owner)
	,_forwardKey(0)
	,_backKey(0)
	,_clockwiseKey(0)
	,_counterClockwiseKey(0)
{

}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	float forwardSpeed = 0.0f;
	if (keyState[_forwardKey])
	{
		forwardSpeed = _maxForwardSpeed;
	}
	if (keyState[_backKey])
	{
		forwardSpeed = -_maxForwardSpeed;
	}
	SetForwardSpeed(forwardSpeed);

	float angularSpeed = 0.0f;
	if (keyState[_clockwiseKey])
	{
		angularSpeed = _maxAngularSpeed;
	}
	if (keyState[_counterClockwiseKey])
	{
		angularSpeed = -_maxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
}