#ifndef LESSON03_H
#define LESSON03_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"

namespace Engine {
	class Lesson03 :public Engine::Game
	{
	public:
		Lesson03(Setting* setting);
		~Lesson03();
		virtual void Init();
		virtual void Update();
		virtual void Render();
	private:
		Engine::Texture* texture = NULL;
		Engine::Sprite* sprite = NULL;
	};
}

#endif

