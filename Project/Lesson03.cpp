#include "Lesson03.h"

Engine::Lesson03::Lesson03(Setting* setting) :Engine::Game(setting)
{

}

Engine::Lesson03::~Lesson03()
{
	delete texture;
	delete sprite;
}

void Engine::Lesson03::Init()
{
	texture = new Texture("turtles.png");
	sprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
	sprite->SetNumXFrames(14);
	sprite->SetNumYFrames(4);
	sprite->AddAnimation("hit", 2, 4);
	sprite->AddAnimation("spikes", 5, 12);
	sprite->AddAnimation("idle-1", 14, 27);
	sprite->AddAnimation("idle-2", 28, 41);
	sprite->AddAnimation("spikes-out", 42, 49);
	sprite->PlayAnim("spikes-out");
	sprite->SetScale(10);
	sprite->SetAnimationDuration(100);
}

void Engine::Lesson03::Update()
{
	sprite->Update(GetGameTime());
}

void Engine::Lesson03::Render()
{
	sprite->Draw();
}

/*
int main(int argc, char** argv) {
	Engine::Setting* setting = new Engine::Setting();
	setting->windowTitle = "Animation Demo";
	setting->screenWidth = 1024;
	setting->screenHeight = 768;
	setting->windowFlag = Engine::WindowFlag::WINDOWED;
	setting->vsync = false;
	setting->targetFrameRate = 0;
	Engine::Game* game = new Engine::Lesson03(setting);
	game->Run();
	delete setting;
	delete game;

	return 0;
}
*/
