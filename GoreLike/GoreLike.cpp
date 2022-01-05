// GoreLike.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "entt.hpp"
#include "SystemClasses.h"
#include "SDL.h"
#include "FileUtil.h"
#include "GameStates.h"
#include "MapComponents.h"
#include <iostream>

int main(int argc, char* args[])
{
    entt::registry reg;
    TestGameState gs{reg};
    gs.start(reg);
    return 0;
}
