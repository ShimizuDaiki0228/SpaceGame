#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyState);

	const Vector2& GetPosition() const { return _position; }
	void SetPosition(const Vector2& pos) { _position = pos; }
	float GetScale() const { return _scale; }
	void SetScale(float scale) { _scale = scale; }
	float GetRotation() const { return _rotation; }
	void SetRotation(float rotation) { _rotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(_rotation), -Math::Sin(_rotation)); }

	State GetState() const { return _state; }
	void SetState(State state) { _state = state; }

	class Game* GetGame() { return _game; }


	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	State _state;

	Vector2 _position;
	float _scale;
	float _rotation;

	std::vector<class Component*> _components;
	class Game* _game;
};