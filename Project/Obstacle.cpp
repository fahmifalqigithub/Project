#include "Obstacle.h"
#include "Sprite.h"

Engine::Obstacle::Obstacle(Sprite* sprite)
{
	this->sprite = sprite;
	state = Engine::ObstacleState::DIE;
	idleDur = 0;
	idleTime = 1000;
}

Engine::Obstacle::~Obstacle()
{

}

void Engine::Obstacle::Update(float deltaTime)
{
	if (state == Engine::ObstacleState::DIE) {
		return;
	}

	float x = GetX();
	float y = GetY();
	float yVelocity = 0;
	float xVelocity = 0.2;

	if (Engine::ObstacleState::ACTIVE == state && x <= -200) {
		state = Engine::ObstacleState::IDLE;
	}

	if (state == Engine::ObstacleState::IDLE) {
		yVelocity = 0;
		xVelocity = 0;
		if (idleDur >= idleTime) {
			state = Engine::ObstacleState::DIE;
			idleDur = 0;
		}
		idleDur += deltaTime;
	}

	if (state == Engine::ObstacleState::DIE) {
		yVelocity = 0;
		xVelocity = 0;

		sprite->SetPosition(-100, -100);
		return;
	}

	y += yVelocity * deltaTime;
	x -= xVelocity * deltaTime;
	sprite->SetPosition(x, y);
	sprite->Update(deltaTime);
}

void Engine::Obstacle::Draw()
{
	if (state == Engine::ObstacleState::DIE) {
		return;
	}

	sprite->Draw();
}



void Engine::Obstacle::SetPosition(float x, float y)
{
	sprite->SetPosition(x, y);
}

void Engine::Obstacle::SetActive()
{
	this->state = Engine::ObstacleState::ACTIVE;
}

void Engine::Obstacle::SetIdle()
{
	this->state = Engine::ObstacleState::IDLE;
}

void Engine::Obstacle::SetDie()
{
	this->state = Engine::ObstacleState::DIE;
}

float Engine::Obstacle::GetWidth()
{
	return sprite->GetScaleWidth();
}

float Engine::Obstacle::GetHeight()
{
	return sprite->GetScaleHeight();
}

bool Engine::Obstacle::IsDie()
{
	return Engine::ObstacleState::DIE == state;
}

bool Engine::Obstacle::IsActive()
{
	return Engine::ObstacleState::ACTIVE == state;
}

float Engine::Obstacle::GetX()
{
	return sprite->GetPosition().x;
}

float Engine::Obstacle::GetY()
{
	return sprite->GetPosition().y;
}

