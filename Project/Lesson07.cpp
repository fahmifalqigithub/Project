#include "Lesson07.h"

Engine::Lesson07::Lesson07(Setting* setting) :Engine::Game(setting), lastShootTime(0), shootCooldown(500)
{
	// Set to mainmenu
	screenState = Engine::ScreenState::MAIN_MENU;
	text = nullptr;
	highestScore = LoadHighestScoreFromStorage();
}

Engine::Lesson07::~Lesson07()
{
	delete dotTexture;
	delete bulletTexture;
	delete monsterSprite;
	delete music;
	delete sound;
	delete text;
	delete crosshairTexture;
	delete crosshairSprite;
	delete dotSprite1;
	delete dotSprite2;
	delete dotSprite3;
	delete dotSprite4;
	delete spikeTexture;
}

void Engine::Lesson07::Init()
{
	InitMainMenu();
	InitInGame();
}

void Engine::Lesson07::Update()
{

	if (Engine::ScreenState::MAIN_MENU == screenState) {
		UpdateMainMenu();
	}
	else if (Engine::ScreenState::IN_GAME == screenState) {
		UpdateInGame();
	}
}

void Engine::Lesson07::Render()
{
	if (Engine::ScreenState::MAIN_MENU == screenState) {
		RenderMainMenu();
	}
	else if (Engine::ScreenState::IN_GAME == screenState) {
		RenderInGame();
	}
}

void Engine::Lesson07::InitMainMenu()
{

	Texture* backgroundTexture = new Texture("bg2.png"); // Ganti "mainmenu_bg.png" dengan nama file gambar Anda
	backgroundSprite = new Sprite(backgroundTexture, defaultSpriteShader, defaultQuad);
	backgroundSprite->SetSize(setting->screenWidth, setting->screenHeight);

	// Create a Texture
	Texture* texture = new Texture("buttons.png");

	// Create Sprites
	Sprite* playSprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
	playSprite->SetNumXFrames(6);
	playSprite->SetNumYFrames(1);
	playSprite->AddAnimation("normal", 5, 5);
	playSprite->AddAnimation("hover", 3, 4);
	playSprite->AddAnimation("press", 3, 4);
	playSprite->SetAnimationDuration(400);

	Sprite* exitSprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
	exitSprite->SetNumXFrames(6);
	exitSprite->SetNumYFrames(1);
	exitSprite->AddAnimation("normal", 2, 2);
	exitSprite->AddAnimation("hover", 0, 1);
	exitSprite->AddAnimation("press", 0, 1);
	exitSprite->SetAnimationDuration(400);

	//Create Buttons
	Button* playButton = new Button(playSprite, "play");
	playButton->SetPosition((setting->screenWidth / 2) - (playSprite->GetScaleWidth() / 2),
		400);
	buttons.push_back(playButton);
	Button* exitButton = new Button(exitSprite, "exit");
	exitButton->SetPosition((setting->screenWidth / 2) - (exitSprite->GetScaleWidth() / 2),
		250);
	buttons.push_back(exitButton);

	// Set play button into active button
	currentButtonIndex = 0;
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);

	// Create Text
	text = new Text("8-bit Arcade In.ttf", 100, defaultTextShader);
	text->SetText("The Spawning Turtle");
	text->SetPosition(60, setting->screenHeight - 100);
	text->SetColor(235, 229, 52);

	// Add input mappings
	inputManager->AddInputMapping("next", SDLK_DOWN);
	inputManager->AddInputMapping("prev", SDLK_UP);
	inputManager->AddInputMapping("press", SDLK_RETURN);

}

void Engine::Lesson07::InitInGame()
{
	// Inisialisasi objek peluru
	bulletTexture = new Texture("bullet2.png");

	// Inisialisasi kekeran (crosshair)
	crosshairTexture = new Texture("crosshair.png");
	crosshairSprite = new Sprite(crosshairTexture, defaultSpriteShader, defaultQuad);
	SDL_ShowCursor(SDL_DISABLE); // Sembunyikan kursor default

	// Inisialisasi platform
	Texture* platformTexture = new Texture("tileset.png");
	vec2 start = vec2(200, 90);
	// Platform baris pertama
	Sprite* platformSprite1 = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
	platformSprite1->SetPosition(start.x, start.y);
	platformSprite1->SetSize(150, 32);
	platformSprite1->SetBoundingBoxSize(70.0f, 25.0f);
	platforms.push_back(platformSprite1);

	// Platform baris kedua
	Sprite* platformSprite2 = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
	platformSprite2->SetPosition(start.x, start.y + 200);
	platformSprite2->SetSize(150, 32);
	platformSprite2->SetBoundingBoxSize(70.0f, 25.0f);
	platforms.push_back(platformSprite2);

	// Platform baris ketiga
	Sprite* platformSprite3 = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
	platformSprite3->SetPosition(start.x + 200, start.y + 100);
	platformSprite3->SetSize(150, 32);
	platformSprite3->SetBoundingBoxSize(70.0f, 25.0f);
	platforms.push_back(platformSprite3);

	// Platform baris keempat
	Sprite* platformSprite4 = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
	platformSprite4->SetPosition(start.x + 400, start.y);
	platformSprite4->SetSize(150, 32);
	platformSprite4->SetBoundingBoxSize(70.0f, 25.0f);
	platforms.push_back(platformSprite4);

	// Platform baris kelima
	Sprite* platformSprite5 = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
	platformSprite5->SetPosition(start.x + 400, start.y + 200);
	platformSprite5->SetSize(150, 32);
	platformSprite5->SetBoundingBoxSize(70.0f, 25.0f);
	platforms.push_back(platformSprite5);

	// Buat latar belakang
	Texture* bgTexture = new Texture("bg2.png");
	backgroundSprite = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
	backgroundSprite->SetSize(setting->screenWidth, setting->screenHeight);

	//Set spawn condition
	spawnDuration = 0;
	maxSpawnTime = 300;
	numObjectsInPool = 3;
	numObjectPerSpawn = 3;

	// Load enemy texture
    spikeTexture = new Texture("wizard.png");

    // Create enemy sprites
    for (int i = 0; i <= numObjectsInPool; i++) {
        Sprite* spikeSprite = new Sprite(spikeTexture, defaultSpriteShader, defaultQuad);
        Obstacle* tempSpike = new Obstacle(spikeSprite);
        spikeSprite->SetNumXFrames(6); // Jumlah frame dalam animasi horizontal
        spikeSprite->SetNumYFrames(1); // Jumlah frame dalam animasi vertikal
        spikeSprite->SetAnimationDuration(200); // Durasi untuk satu putaran animasi (dalam milidetik)
        tempSpike->SetPosition(1500, (i + 1) * 100); // Atur posisi awal
        tempSpike->SetActive();
        spikeSprite->SetBoundingBoxSize(50.0f, 50.0f);
        cout << "SPIKE " << i << " CREATED" << endl;
        spikes.push_back(tempSpike);
    }

	// This dot sprite is for visual debugging (to see the actual bounding box) 
	dotTexture = new Texture("dot.png");
	dotSprite1 = new Sprite(dotTexture, defaultSpriteShader, defaultQuad);
	dotSprite2 = new Sprite(dotTexture, defaultSpriteShader, defaultQuad);
	dotSprite3 = new Sprite(dotTexture, defaultSpriteShader, defaultQuad);
	dotSprite4 = new Sprite(dotTexture, defaultSpriteShader, defaultQuad);


	// Monster sprite
	Texture* monsterTexture = new Texture("gunner.png");
	monsterSprite = new Sprite(monsterTexture, defaultSpriteShader, defaultQuad);
	monsterSprite->SetPosition(0, 0);
	monsterSprite->SetNumXFrames(6);
	monsterSprite->SetNumYFrames(3);
	monsterSprite->SetAnimationDuration(200);
	monsterSprite->SetScale(2.0f);
	monsterSprite->PlayAnim("idle");
	monsterSprite->AddAnimation("jump", 6, 7);
	monsterSprite->AddAnimation("idle", 0, 4);
	monsterSprite->AddAnimation("run", 13, 17);

	// Inisialisasi musik dan suara
	music = new Music("bgsound.ogg");
	music->SetVolume(20);
	music->Play(true);

	sound = new Sound("jump2.wav");
	sound->SetVolume(50);

	sound2 = new Sound("gunshot.wav");
	sound2->SetVolume(40);

	soundEnemyHit = new Sound("death.wav");
	soundEnemyHit->SetVolume(700);

	text = new Text("lucon.ttf", 24, defaultTextShader);
	text->SetScale(1.0f);
	text->SetColor(255, 255, 255);
	text->SetPosition(0, setting->screenHeight - (text->GetFontSize() * text->GetScale()));

	SetBackgroundColor(102, 195, 242);

	// Atur kontrol input
	inputManager->AddInputMapping("Run Right", SDLK_d);
	inputManager->AddInputMapping("Run Left", SDLK_a);
	inputManager->AddInputMapping("Jump", SDLK_w);
	inputManager->AddInputMapping("Shoot", SDL_BUTTON_LEFT); // Klik kiri mouse
	inputManager->AddInputMapping("Quit", SDLK_ESCAPE);

	// Add input mappings
	inputManager->AddInputMapping("mainmenu", SDLK_ESCAPE);
}

void Engine::Lesson07::RenderMainMenu()
{
	backgroundSprite->Draw();

	// Render all buttons
	for (Button* b : buttons) {
		b->Draw();
	}
	// Render title 
	text->Draw();

	
}

void Engine::Lesson07::RenderInGame()
{
	// Render latar belakang
	backgroundSprite->Draw();
	text->Draw();

	// Render platform
	for (Sprite* s : platforms) {
		s->Draw();
	}

	// Render monster
	monsterSprite->Draw();

	//Update Obstacle
	for (Obstacle* o : spikes) {
		//cout << "SPIKE UPDATE CALLED" << endl;
		o->Draw();
	}

	// Render peluru
	for (Sprite* bullet : bullets) {
		bullet->Draw();
	}

	// Render kekeran
	crosshairSprite->Draw();

	if (debug) {
		dotSprite1->Draw();
		dotSprite2->Draw();
		dotSprite3->Draw();
		dotSprite4->Draw();
	}
}

void Engine::Lesson07::UpdateMainMenu()
{
	// Set background
	SetBackgroundColor(52, 155, 235);

	if (inputManager->IsKeyReleased("next")) {
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Next Button
		currentButtonIndex = currentButtonIndex < buttons.size() - 1 ? currentButtonIndex + 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	if (inputManager->IsKeyReleased("prev")) {
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Prev Button
		currentButtonIndex = currentButtonIndex > 0 ? currentButtonIndex - 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	if (inputManager->IsKeyReleased("press")) {
		// Set current button to press state
		Button* b = buttons[currentButtonIndex];
		b->SetButtonState(Engine::ButtonState::PRESS);
		// If play button then go to InGame, exit button then exit
		if ("play" == b->GetButtonName()) {
			// Reset game state before starting
			ResetGameState();
			screenState = Engine::ScreenState::IN_GAME;
		}
		else if ("exit" == b->GetButtonName()) {
			state = Engine::State::EXIT;
		}
	}

	// Update All buttons
	for (Button* b : buttons) {
		b->Update(GetGameTime());
	}
}

void Engine::Lesson07::UpdateInGame()
{
	// Back to main menu
	if (inputManager->IsKeyReleased("Quit")) {
		screenState = Engine::ScreenState::MAIN_MENU;
		return;
	}

	text->SetText("FPS = " + to_string(currentFrameRate) +
		" Score: " + to_string(score) +
		" a & d - w - left mouse to Move, Jump, Shoot");

	// Spawn objects
	if (spawnDuration >= maxSpawnTime) {
		RespawnSpikes();
		spawnDuration = 0;
	}

	monsterSprite->Update(GetGameTime());
	float x = monsterSprite->GetPosition().x;
	float y = monsterSprite->GetPosition().y;
	float velocity = 0.07f;
	monsterSprite->PlayAnim("idle");

	// Pergerakan karakter
	vec2 oldMonsterPos = monsterSprite->GetPosition();

	if (inputManager->IsKeyPressed("Run Right")) {
		monsterSprite->PlayAnim("run");
		x += 0.2f * GetGameTime();
		monsterSprite->SetFlipHorizontal(false);
	}

	if (inputManager->IsKeyPressed("Run Left")) {
		monsterSprite->PlayAnim("run");
		x -= 0.2f * GetGameTime();
		monsterSprite->SetFlipHorizontal(true);
	}

	monsterSprite->SetPosition(x, y);

	// Periksa kolisi
	for (Sprite* s : platforms) {
		if (s->GetBoundingBox()->CollideWith(monsterSprite->GetBoundingBox())) {
			x = oldMonsterPos.x;
			monsterSprite->SetPosition(x, y);
			break;
		}
	}

	// Lompat
	if (inputManager->IsKeyPressed("Jump") && !jump) {
		monsterSprite->PlayAnim("jump");
		float ratio = (GetGameTime() / 16.7f);
		gravity = 0.16f * ratio;
		yVelocity = 1.8f;
		jump = true;
		sound->Play(false);
	}

	if (y > 0) {
		yVelocity -= gravity;
	}
	else if (y < 0) {
		jump = false;
		yVelocity = 0;
		y = 0;
	}

	y += yVelocity * GetGameTime();
	monsterSprite->SetPosition(x, y);

	// Periksa kolisi dengan platform
	for (Sprite* s : platforms) {
		if (s->GetBoundingBox()->CollideWith(monsterSprite->GetBoundingBox())) {
			y = oldMonsterPos.y;
			monsterSprite->SetPosition(x, y);
			yVelocity = 0;
			jump = false;
			break;
		}
	}

	// Update posisi kekeran mengikuti posisi mouse
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mouseY = setting->screenHeight - mouseY; // Membalik koordinat Y
	crosshairSprite->SetPosition((float)mouseX, (float)mouseY);

	// Menembak peluru
	Uint32 currentTime = SDL_GetTicks();
	if (inputManager->IsMouseButtonPressed("Shoot") && (currentTime - lastShootTime) > shootCooldown) {
		lastShootTime = currentTime;
		sound2->Play(false);
		vec2 monsterPos = monsterSprite->GetPosition();
		vec2 mousePos = vec2((float)mouseX, (float)mouseY);
		vec2 direction = normalize(mousePos - monsterPos);

		Sprite* bullet = new Sprite(bulletTexture, defaultSpriteShader, defaultQuad);
		bullet->SetPosition(monsterSprite->GetPosition().x, monsterSprite->GetPosition().y + monsterSprite->GetScaleHeight() / 2);
		bullet->SetNumXFrames(1);
		bullet->SetNumYFrames(1);
		bullet->SetDirection(direction);
		bullets.push_back(bullet);
	}

	for (auto it = bullets.begin(); it != bullets.end();) {
		Sprite* bullet = *it;
		vec2 bulletPos = bullet->GetPosition();
		vec2 bulletDir = bullet->GetDirection();
		bullet->SetPosition(bulletPos.x + bulletDir.x * 0.5f * GetGameTime(), bulletPos.y + bulletDir.y * 0.5f * GetGameTime());

		bool bulletCollision = false;
		// Hanya cek tabrakan dengan musuhwd
		for (Obstacle* o : spikes) {
			if (o->IsActive() && o->sprite->GetBoundingBox()->CollideWith(bullet->GetBoundingBox())) {
				bulletCollision = true;
				delete bullet;
				it = bullets.erase(it);
				score++;
				o->SetDie();
				soundEnemyHit->Play(false);
				break;
			}
		}

		if (!bulletCollision) {
			it++;
		}
	}

	if (debug) {
		BoundingBox* bb = monsterSprite->GetBoundingBox();
		dotSprite1->SetPosition(bb->GetVertices()[0].x - (0.5f * dotSprite1->GetScaleWidth()),
			bb->GetVertices()[0].y - (0.5f * dotSprite1->GetScaleHeight()));
		dotSprite2->SetPosition(bb->GetVertices()[1].x - (0.5f * dotSprite2->GetScaleWidth()),
			bb->GetVertices()[1].y - (0.5f * dotSprite2->GetScaleHeight()));
		dotSprite3->SetPosition(bb->GetVertices()[2].x - (0.5f * dotSprite3->GetScaleWidth()),
			bb->GetVertices()[2].y - (0.5f * dotSprite3->GetScaleHeight()));
		dotSprite4->SetPosition(bb->GetVertices()[3].x - (0.5f * dotSprite3->GetScaleWidth()),
			bb->GetVertices()[3].y - (0.5f * dotSprite3->GetScaleHeight()));
	}

	for (Obstacle* o : spikes) {
		o->Update(GetGameTime());

		if (o->GetX() + o->GetWidth() < 0) {
			screenState = Engine::ScreenState::MAIN_MENU; // Kembali ke menu utama jika game over
			return;
		}
	}

	spawnDuration += GetGameTime();
	monsterSprite->Update(GetGameTime());
	crosshairSprite->Update(GetGameTime());
}

void Engine::Lesson07::ResetGameState()
{
	// Reset all game variables and objects to their initial state
	score = 0;
	screenState = Engine::ScreenState::MAIN_MENU; // Reset screen state to main menu

	// Reset character position and velocity
	monsterSprite->SetPosition(0, 0);
	yVelocity = 0;
	jump = false;

	// Reset bullets
	for (Sprite* bullet : bullets) {
		delete bullet;
	}
	bullets.clear();

	// Reset obstacles
	for (Obstacle* o : spikes) {
		o->SetPosition(1500, 0); // Reset position
		o->SetDie(); // Set obstacle to inactive state
	}

	// Reset crosshair position
	int centerX = setting->screenWidth / 2;
	int centerY = setting->screenHeight / 2;
	crosshairSprite->SetPosition(centerX, centerY);

	// Save highest score to storage
	SaveHighestScoreToStorage(highestScore);
}

int Engine::Lesson07::LoadHighestScoreFromStorage()
{
	// Load highest score from storage (e.g., from a file)
	int highestScore = 0;
	std::ifstream file("highest_score.txt");
	if (file.is_open()) {
		file >> highestScore;
		file.close();
	}
	return highestScore;
}

void Engine::Lesson07::SaveHighestScoreToStorage(int highestScore)
{
	// Save highest score to storage (e.g., to a file)
	std::ofstream file("highest_score.txt");
	if (file.is_open()) {
		file << highestScore;
		file.close();
	}
}

void Engine::Lesson07::RespawnSpikes()
{
	//Find Die object to reuse for spawning
	int spawnCount = 0;
	for (Obstacle* o : spikes) {
		if (spawnCount == numObjectPerSpawn) {
			break;
		}
		if (o->IsDie()) {
			// Set state to spawn
			o->SetActive();
			// Random spawn position
			int min = 0;
			int max = setting->screenHeight - o->GetHeight();
			float y = rand() % (max - min + 1) + min;
			//float y = setting->screenHeight + o->GetHeight();
			//float x = (setting->screenWidth / 2);
			float x = (setting->screenWidth);
			o->SetPosition(x, y);
			spawnCount++;
		}
	}
}


int main(int argc, char** argv) {
	Engine::Setting* setting = new Engine::Setting();
	setting->windowTitle = "GUI Demo";
	setting->screenWidth = 1024;
	setting->screenHeight = 768;
	setting->windowFlag = Engine::WindowFlag::EXCLUSIVE_FULLSCREEN;
	setting->vsync = false;
	setting->targetFrameRate = 0;
	Engine::Game* game = new Engine::Lesson07(setting);
	game->Run();
	delete setting;
	delete game;

	return 0;
}