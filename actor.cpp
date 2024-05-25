#include "actor.h"
#include "game.h"
#include "component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:_state(EActive)
	, _position(Vector2::Zero)
	, _scale(1.0f)
	, _rotation(0.0f)
	, _game(game)
{
	_game->AddActor(this);
}

Actor::~Actor()
{
	_game->RemoveActor(this);

	while (!_components.empty())
	{
		delete _components.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (_state == EActive)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : _components)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput(const uint8_t* keyState)
{
	if (_state == EActive)
	{
		for (auto comp : _components)
		{
			comp->ProcessInput(keyState);
		}

		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}

void Actor::AddComponent(Component* component)
{

	int myOrder = component->GetUpdateOrder();
	auto iter = _components.begin();
	for (;
		iter != _components.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	_components.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(_components.begin(), _components.end(), component);
	if (iter != _components.end())
	{
		_components.erase(iter);
	}
}