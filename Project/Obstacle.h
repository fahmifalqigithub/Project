#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Sprite.h"

namespace Engine {
	enum class ObstacleState {
		ACTIVE,
		IDLE,
		DIE
	};
	class Obstacle
	{
	public:
		Obstacle(Sprite* sprite);
		~Obstacle();
		void Update(float deltaTime);
		void Draw();
		void SetPosition(float x, float y);
		void SetActive();
		void SetIdle();
		float GetWidth();
		float GetHeight();
		bool IsDie();
		bool IsActive();
		float GetX();
		float GetY();
		void SetDie();
		Sprite* sprite = NULL;
	protected:
		
		ObstacleState state;
		float idleDur = 0, idleTime = 0, x = 0, y = 0;
	};

}

#endif