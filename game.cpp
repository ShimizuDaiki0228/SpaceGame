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
	, _startVanishTime(0)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	_window = SDL_CreateWindow("Game Programming in C++ (Chapter 3)", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), _ticksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - _ticksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	_ticksCount = SDL_GetTicks();

	_updatingActors = true;
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}
	_updatingActors = false;

	for (auto pending : _pendingActors)
	{
		_actors.emplace_back(pending);
	}
	_pendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : _actors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}

	_startVanishTime += deltaTime;
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(_renderer, 220, 220, 220, 255);
	SDL_RenderClear(_renderer);

	
	for (auto sprite : _sprites)
	{
		Actor* actor = sprite->GetOwner();
		if (actor && actor->GetType() == "Ship")
		{
			float diffTime = _startVanishTime - (int)_startVanishTime;
			if (actor->IsInvincibility() && (IsVanishing(0.0f, 0.25f, diffTime) || IsVanishing(0.5f, 0.75f, diffTime)))
			{
				continue;
			}
		}
		sprite->Draw(_renderer);
	}

	
	
	SDL_RenderPresent(_renderer);
}

bool IsVanishing(float min, float max, float diffTime)
{
	if (diffTime > min && diffTime <= max) return true;
	return false;
}

void Game::LoadData()
{
	//プレイヤーの船を作成
	_ship = new Ship(this);
	_ship->SetPosition(Vector2(512.0f, 384.0f));
	_ship->SetRotation(Math::PiOver2);

	// 惑星を作成
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	// アクターを削除
	while (!_actors.empty())
	{
		delete _actors.back();
	}

	// テクスチャを破棄する
	for (auto i : _textures)
	{
		SDL_DestroyTexture(i.second);
	}
	_textures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// テクスチャが存在するか
	auto iter = _textures.find(fileName);
	if (iter != _textures.end())
	{
		tex = iter->second;
	}
	else
	{
		// ファイルからロードする
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// surfaceからテクスチャを取得
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
	auto iter = std::find(_pendingActors.begin(), _pendingActors.end(), actor);
	if (iter != _pendingActors.end())
	{
		std::iter_swap(iter, _pendingActors.end() - 1);
		_pendingActors.pop_back();
	}

	iter = std::find(_actors.begin(), _actors.end(), actor);
	if (iter != _actors.end())
	{
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

	_sprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(_sprites.begin(), _sprites.end(), sprite);
	_sprites.erase(iter);
}