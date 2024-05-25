#include "laser.h"
#include "spriteComponent.h"
#include "game.h"
#include "moveComponent.h"
#include "circleComponent.h"
#include "asteroid.h"

Laser::Laser(Game* game)
	:Actor(game)
	,_deathTime(1.0f)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.0f);

	_circle = new CircleComponent(this);
	_circle->SetRadius(11.0f);

}

void Laser::UpdateActor(float deltaTime)
{
	_deathTime -= deltaTime;

	if (_deathTime < 0)
	{
		SetState(EDead);
	}
	else
	{
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*_circle, *(ast->GetCircle())))
			{
				SetState(EDead);
				ast->SetState(EDead);
				break;
			}
		}
	}
}