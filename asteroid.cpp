#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
	,_circle(nullptr)
{
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	//SpriteComponent‚ðì¬‚·‚é
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//MoveComponent‚ðì¬‚·‚é
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);

	//CricleComponent‚ðì¬‚·‚é
	_circle = new CircleComponent(this);
	_circle->SetRadius(40.0f);

	//asteroid‚ðgame‚É’Ç‰Á‚·‚é
	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}