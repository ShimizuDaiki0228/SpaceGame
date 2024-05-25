#pragma once
#include "Component.h"
#include "SDL.h"
class SpriteComponent : public Component
{
public:
	// (Lower draw order corresponds with further back)
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const { return _drawOrder; }
	int GetTexHeight() const { return _texHeight; }
	int GetTexWidth() const { return _texWidth; }

	Actor* GetOwner() const { return _owner; }
protected:
	SDL_Texture* _texture;
	int _drawOrder;
	int _texWidth;
	int _texHeight;
};