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

	// Game-specific (add/remove asteroid)
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return _asteroids; }
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
};