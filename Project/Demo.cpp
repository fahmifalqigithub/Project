#include "Demo.h"

using namespace Engine;

Demo::Demo(Setting* setting) : Game(setting), lastShootTime(0), shootCooldown(500) // 0.5 detik cooldown dalam milidetik
{
}

Demo::~Demo()
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

void Demo::Init()
{
    // Inisialisasi objek peluru
    bulletTexture = new Texture("bullet2.png");

    // Inisialisasi kekeran (crosshair)
    crosshairTexture = new Texture("crosshair.png");
    crosshairSprite = new Sprite(crosshairTexture, defaultSpriteShader, defaultQuad);
    SDL_ShowCursor(SDL_DISABLE); // Sembunyikan kursor default

    // Inisialisasi platform
    Texture* platformTexture = new Texture("platform.png");
    vec2 start = vec2(200, 90);
    for (int i = 0; i < 8; i++) {
        Sprite* platformSprite = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
        platformSprite->SetPosition(start.x + i * 210, start.y + i * 60);
        platformSprite->SetSize(150, 32);
        platforms.push_back(platformSprite);
    }

    // Buat latar belakang
    Texture* bgTexture = new Texture("bg.png");
    backgroundSprite = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
    backgroundSprite->SetSize(setting->screenWidth, setting->screenHeight);

    //Set spawn condition
    spawnDuration = 0;
    maxSpawnTime = 300;
    numObjectsInPool = 3;
    numObjectPerSpawn = 3;

    //Create obstacle
    spikeTexture = new Texture("enemy.png");
    for (int i = 0; i <= numObjectsInPool; i++) {
        Sprite* spikeSprite = new Sprite(spikeTexture, defaultSpriteShader, defaultQuad);
        Obstacle* tempSpike = new Obstacle(spikeSprite);
        spikeSprite->SetNumXFrames(4);
        spikeSprite->SetNumYFrames(1);
        tempSpike->SetPosition(1500, (i + 1) * 100);
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
    music = new Music("2021-08-16_-_8_Bit_Adventure_-_www.FesliyanStudios.com.ogg");
    music->SetVolume(10);
    music->Play(true);

    sound = new Sound("jump.wav");
    sound->SetVolume(100);

    sound2 = new Sound("gunshot.wav");
    sound->SetVolume(100);

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
}

void Demo::Update()
{
    if (inputManager->IsKeyReleased("Quit")) {
        state = State::EXIT;
        return;
    }

    text->SetText("FPS = " + to_string(currentFrameRate) +
        " Score: " + to_string(score) +
        " a & d - w - left mouse to Move, Jump, Shoot");

    //Spawn objects
    if (spawnDuration >= maxSpawnTime) {
        RespawnSpikes();
        spawnDuration = 0;
    }

    monsterSprite->Update(GetGameTime());
    float x = monsterSprite->GetPosition().x;
    float y = monsterSprite->GetPosition().y;
    float velocity = 0.07f;
    // s = v * t;
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
        // Hitung arah peluru
        vec2 monsterPos = monsterSprite->GetPosition();
        vec2 mousePos = vec2((float)mouseX, (float)mouseY);
        vec2 direction = normalize(mousePos - monsterPos);

        // Buat objek peluru
        Sprite* bullet = new Sprite(bulletTexture, defaultSpriteShader, defaultQuad);
        bullet->SetPosition(monsterSprite->GetPosition().x, monsterSprite->GetPosition().y + monsterSprite->GetScaleHeight() / 2);
        bullet->SetNumXFrames(1);
        bullet->SetNumYFrames(1);
        bullet->SetDirection(direction);
        bullets.push_back(bullet);
        
    }

    // Perbarui posisi peluru
    for (auto it = bullets.begin(); it != bullets.end();) {
        Sprite* bullet = *it;
        vec2 bulletPos = bullet->GetPosition();
        vec2 bulletDir = bullet->GetDirection();
        bullet->SetPosition(bulletPos.x + bulletDir.x * 0.5f * GetGameTime(), bulletPos.y + bulletDir.y * 0.5f * GetGameTime());

        // Periksa tabrakan peluru dengan platform
        bool bulletCollision = false;
        for (Sprite* s : platforms) {
            if (s->GetBoundingBox()->CollideWith(bullet->GetBoundingBox())) {
                bulletCollision = true;
                // Hapus peluru
                it = bullets.erase(it);
                break;
            }
        }

        // Periksa tabrakan peluru dengan spike
       if (!bulletCollision) {
        for (Obstacle* o : spikes) {
            if (o->IsActive() && o->sprite->GetBoundingBox()->CollideWith(bullet->GetBoundingBox())) {
                bulletCollision = true;
                // Hapus peluru jika mengenai obstacle
                delete bullet;
                it = bullets.erase(it);
                score++;

                o->SetDie();
                break;
            }
        }
    }

        if (!bulletCollision) {
            it++;
        }
    }

    if (debug) {
        //Shape for debug
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

    // Update obstacle
    for (Obstacle* o : spikes) {
        o->Update(GetGameTime());

        // Periksa apakah obstacle melewati batas kiri layar
        if (o->GetX() + o->GetWidth() < 0) {
            state = State::EXIT; // Atur status game ke EXIT atau bisa menggunakan GAMEOVER jika Anda memiliki status seperti itu
            return;
        }
    }

    //count spawn duration
    spawnDuration += GetGameTime();

    // Perbarui animasi monster
    monsterSprite->Update(GetGameTime());
    crosshairSprite->Update(GetGameTime());
}

void Demo::Render()
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

void Demo::RespawnSpikes() {
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


//int main(int argc, char** argv) {
//    Setting* setting = new Setting();
//    setting->windowTitle = "Platform Jump";
//    setting->screenWidth = 1366;
//    setting->screenHeight = 768;
//    setting->windowFlag = WindowFlag::WINDOWED;
//    setting->vsync = true;
//    Game* game = new Demo(setting);
//    game->Run();
//    delete setting;
//    delete game;
//
//    return 0;
//}
