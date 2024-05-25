#pragma once
#include "SDL.h"
#include <stdio.h>
#include <unordered_map>
#include <string>
#include <vector>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return _asteroids; }

	int GetWindowWidth() { return WINDOW_WIDTH; }
	int GetWindowHeight() { return WINDOW_HEIGHT; }

	void ReSetVanishTime() { _startVanishTime = 0; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, SDL_Texture*> _textures;

	std::vector<class Actor*> _actors;
	std::vector<class Actor*> _pendingActors;

	std::vector<class SpriteComponent*> _sprites;

	SDL_Window* _window;
	SDL_Renderer* _renderer;
	Uint32 _ticksCount;
	bool _isRunning;

	bool _updatingActors;

	class Ship* _ship;
	std::vector<class Asteroid*> _asteroids;

	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 764;

	float _startVanishTime;
};

bool IsVanishing(float min, float max, float diffTime);
