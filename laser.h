#pragma once
#include "actor.h"


class Laser : public Actor
{
public:
	Laser(class Game* game);

	void UpdateActor(float deltaTime) override;

private:
	class CircleComponent* _circle;
	float _deathTime;
};