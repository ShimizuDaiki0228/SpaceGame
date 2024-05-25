#include "ship.h"
#include "spriteComponent.h"
#include "inputComponent.h"
#include "game.h"
#include "laser.h"
#include "asteroid.h"
#include "math.h"

Ship::Ship(Game* game)
	:Actor(game)
	, _laserCooldown(0.0f)
	, _invincibilityTime(0.0f)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	_circle = new CircleComponent(this);
	_circle->SetRadius(GetScale());

	_resetPos.Set(GetGame()->GetWindowWidth() / 2, GetGame()->GetWindowHeight() / 2);
}

void Ship::UpdateActor(float deltaTime)
{
	_laserCooldown -= deltaTime;
	_invincibilityTime -= deltaTime;

	IntersectAsteroid();
}

void Ship::IntersectAsteroid()
{
	if (_invincibilityTime < 0.0f)
	{
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*_circle, *(ast->GetCircle())))
			{
				GetGame()->ReSetVanishTime();
				_invincibilityTime = 2.0f;
				SetPosition(_resetPos);
			}
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && _laserCooldown <= 0.0f)
	{
		// レーザーを作成して、場所と回転を設定する
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// レーザーのクールダウン
		_laserCooldown = 0.5f;
	}
}