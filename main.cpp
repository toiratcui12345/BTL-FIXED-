#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
#include "defines.h"

#include "game.h"
#include "objects.h"
#include "graphics.h"

using namespace std;

int main(int argc, char* argv[]) {
    Application app;
    app.appInit();

    app.appLoop();
    return 0;
}
