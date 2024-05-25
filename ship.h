#pragma once
#include "Actor.h"
#include "circleComponent.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;

	const char* GetType() const override { return "Ship"; }
	bool IsInvincibility() const override { return _invincibilityTime > 0; }
private:
	void IntersectAsteroid();

	float _laserCooldown;
	CircleComponent* _circle;

	Vector2 _resetPos;
	float _invincibilityTime;
};