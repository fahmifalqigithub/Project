#ifndef Lesson07_H
#define Lesson07_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "Obstacle.h"
#include "GameObject.h"
#include "Sound.h"
#include "Music.h"
#include <vector>

namespace Engine {
    enum class ScreenState {
        MAIN_MENU,
        IN_GAME,
        GAME_OVER
    };
    class Lesson07 : public Engine::Game
    {
    public:
        Lesson07(Setting* setting);
        ~Lesson07();
        virtual void Init();
        virtual void Update();
        virtual void Render();
    private:
        int currentButtonIndex = 0;
        ScreenState screenState;
        std::vector<Button*> buttons;
        Text* text;
        void InitMainMenu();
        void InitInGame();
        void RenderMainMenu();
        void RenderInGame();
        void UpdateMainMenu();
        void UpdateInGame();
        void ResetGameState();

        int LoadHighestScoreFromStorage();

        void SaveHighestScoreToStorage(int highestScore);
        
        Sprite* crosshairSprite;
        Texture* crosshairTexture;
        float shootCooldown;
        float lastShootTime;
        Texture* dotTexture = NULL;
        Texture* bulletTexture = NULL;
        vector<Sprite*> platforms;
        vector<Sprite*> bullets; //untuk menyimpan peluru
        vector<Obstacle*> spikes;
        Texture* spikeTexture = NULL;
        Sprite* monsterSprite = NULL;
        Sprite* backgroundSprite = NULL;
        Sprite* dotSprite1 = NULL;
        Sprite* dotSprite2 = NULL;
        Sprite* dotSprite3 = NULL;
        Sprite* dotSprite4 = NULL;
        Music* music = NULL;;
        Sound* sound = NULL;
        Sound* sound2 = NULL;
        Sound* soundEnemyHit = NULL;
        float yVelocity = 0, gravity = 0;
        bool jump = false, debug = false;
        int score = 0; // Tambahkan variabel skor
        int highestScore = 0;
        int obstacleLeftScreenCount = 0;
        void RespawnSpikes();
        float spawnDuration = 0, maxSpawnTime = 0, numObjectsInPool = 0, numObjectPerSpawn = 0;
    };
}

#endif
