#pragma once
#include "component.h"
#include "math.h"

class CircleComponent : public Component
{
public:
	CircleComponent(class Actor* owner);

	void SetRadius(float radius) { _radius = radius; }
	float GetRadius() const;

	const Vector2& GetCenter() const;

private:

	float _radius;
};

bool Intersect(const CircleComponent& a, const CircleComponent& b);