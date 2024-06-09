// Screen.h
#pragma once
#include "Setting.h"
#include "InputManager.h"


namespace Engine {
    class Screen {
    public:
        virtual ~Screen() {}
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
    protected:
        InputManager* inputManager;
        Setting* setting;
    };
}