#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "ship.h"
#include "SpriteComponent.h"
#include "Asteroid.h"
#include "Random.h"

Game::Game()
	:_window(nullptr)
	, _renderer(nullptr)
	, _isRunning(true)
	, _updatingActors(false)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	_window = SDL_CreateWindow("Game Programming in C++ (Chapter 3)", 100, 100, 1024, 768, 0);
	if (!_window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	Random::Init();

	LoadData();

	_ticksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (_isRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			_isRunning = false;
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		_isRunning = false;
	}

	_updatingActors = true;
	for (auto actor : _actors)
	{
		actor->ProcessInput(keyState);
	}
	_updatingActors = false;
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), _ticksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - _ticksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	_ticksCount = SDL_GetTicks();

	// Update all actors
	_updatingActors = true;
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}
	_updatingActors = false;

	// Move any pending actors to mActors
	for (auto pending : _pendingActors)
	{
		_actors.emplace_back(pending);
	}
	_pendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : _actors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(_renderer, 220, 220, 220, 255);
	SDL_RenderClear(_renderer);

	// Draw all sprite components
	for (auto sprite : _sprites)
	{
		sprite->Draw(_renderer);
	}

	SDL_RenderPresent(_renderer);
}

void Game::LoadData()
{
	// Create player's ship
	_ship = new Ship(this);
	_ship->SetPosition(Vector2(512.0f, 384.0f));
	_ship->SetRotation(Math::PiOver2);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!_actors.empty())
	{
		delete _actors.back();
	}

	// Destroy textures
	for (auto i : _textures)
	{
		SDL_DestroyTexture(i.second);
	}
	_textures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = _textures.find(fileName);
	if (iter != _textures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(_renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		_textures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::AddAsteroid(Asteroid* ast)
{
	_asteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(_asteroids.begin(),
		_asteroids.end(), ast);
	if (iter != _asteroids.end())
	{
		_asteroids.erase(iter);
	}
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	if (_updatingActors)
	{
		_pendingActors.emplace_back(actor);
	}
	else
	{
		_actors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(_pendingActors.begin(), _pendingActors.end(), actor);
	if (iter != _pendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, _pendingActors.end() - 1);
		_pendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(_actors.begin(), _actors.end(), actor);
	if (iter != _actors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, _actors.end() - 1);
		_actors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = _sprites.begin();
	for (;
		iter != _sprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	_sprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(_sprites.begin(), _sprites.end(), sprite);
	_sprites.erase(iter);
}