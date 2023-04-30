#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int SCREEN_WIDTH = 270;
int SCREEN_HEIGHT = 270;
SDL_Window *Window = NULL;
SDL_Renderer *Render = NULL;

void init () {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cout << "Can't init SDL: " << SDL_GetError() << endl;
    }

    Window = SDL_CreateWindow("Sandbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (Window == NULL) {
             cout << "Can't create window: " << SDL_GetError() << endl;
    }

    Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (Render == NULL) {
             cout << "Can't create renderer: " << SDL_GetError() << endl;
    }
}
void GenMap (int Map[][100]) {
    for (int x = 0; x < 100; x++) {
            for (int y = 0; y < 100; y++) {
                    Map[y][x] = 1 + (rand() % 7);
            }
    }
}
void DrawMap (int Map[][100]) {
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
    SDL_RenderClear(Render);
    SDL_Rect Rect = {0, 0, 2, 2};
    for (int x = 0; x < 100; x++) {
        for (int y = 0; y < 100; y++) {
                if (Map[y][x] == 1) {
                    SDL_SetRenderDrawColor(Render, 164, 164, 164, 0);
                    Rect.x = x*2;
                    Rect.y = y*2;
                    SDL_RenderFillRect(Render,&Rect);
                }
                if (Map[y][x] == 2 ) {
                    SDL_SetRenderDrawColor(Render, 251, 163, 96, 0);
                    Rect.x = x*2;
                    Rect.y = y*2;
                    SDL_RenderFillRect(Render,&Rect);
                }
                if (Map[y][x] == 3 ) {
                    SDL_SetRenderDrawColor(Render, 0, 149, 182, 0);
                    Rect.x = x*2;
                    Rect.y = y*2;
                    SDL_RenderFillRect(Render,&Rect);
                }
        }
    }
    SDL_RenderPresent(Render);
}
void FillSpace (int Map[][100], int Number, int x1, int y1, int x2, int y2) {
    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
                Map[y][x] = Number;
        }
    }
}
void MoveMap(int Map[][100]) {
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            //Камень
            if (Map[y][x] == 1 && Map[y+1][x] > 2 && y != 99) {
                    Map[y][x] = Map[y+1][x];
                    Map[y+1][x] = 1;
            }
            //Песок
            if (Map[y][x] == 2) {
                    //Падение блока
                if (Map[y+1][x] > 2) {
                    Map[y][x] = Map[y+1][x];
                    Map[y+1][x] = 2;
                } else {
                    //Движение в стороны
                    if (!(Map[y+1][x] < 3 && Map[y+1][x-1] < 3 && Map[y+1][x+1] < 3)) {
                        //Выбор стороны - s
                        int s = rand() % 4;
                        //Движение в <- потом ->
                        if (s == 0) {
                            if (Map[y][x-1] > 2 && x != 0) {
                                Map[y][x] = Map[y][x-1];
                                Map[y][x-1] = 2;
                            } else {
                                if (Map[y][x+1] > 2 && x != 99) {
                                    Map[y][x] = Map[y][x+1];
                                    Map[y][x+1] = 2;
                                }
                            }
                        }
                        //Движение в ->, потом <-
                        if (s == 3) {
                            if (Map[y][x+1] > 2 && x != 99) {
                                Map[y][x] = Map[y][x+1];
                                Map[y][x+1] = 2;
                            }else {
                                if (Map[y][x-1] > 2 && x != 0) {
                                    Map[y][x] = Map[y][x-1];
                                    Map[y][x-1] = 2;
                                }
                            }
                        }
                    }
                }
            }
            //Вода
            if (Map[y][x] == 3) {
                //Падение блока
                if (Map[y+1][x] > 3 && y != 99) {
                    Map[y][x] = 7;
                    Map[y+1][x] = 3;
                } else {
                    if (Map[y][x-1] > 3 || Map[y][x+1] > 3) {
                        int s = rand() % 4;
                        //Движение в <- потом ->
                        if (s == 1) {
                            if (Map[y][x-1] > 3 && x != 0) {
                                Map[y][x] = 7;
                                Map[y][x-1] = 3;
                            } else {
                                if (Map[y][x+1] > 3 && x != 99) {
                                    Map[y][x] = 7;
                                    Map[y][x+1] = 3;
                                }
                            }
                        }
                        //Движение в ->, потом <-
                        if (s == 0) {
                            if (Map[y][x+1] > 3 && x != 99) {
                                Map[y][x] = 7;
                                Map[y][x+1] = 3;
                            }else {
                                if (Map[y][x-1] > 3 && x != 0) {
                                    Map[y][x] = 7;
                                    Map[y][x-1] = 3;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void quit() {
    SDL_DestroyWindow(Window);
    Window = NULL;

    SDL_DestroyRenderer(Render);
    Render = NULL;

    SDL_Quit();
}
int main (int arhc, char ** argv) {
    srand(time(0));
    int Map[100][100];
    int Number = 7;
    int x1 = -1, y1 = -1,
        x2 = -1, y2 = -1;
    init();
    SDL_Event Event;
    bool Done = false;
    GenMap(Map);
    DrawMap(Map);

    while (!Done) {
        while (SDL_PollEvent(&Event) != 0) {
            switch (Event.type)
            {
                case SDL_QUIT:
                    {
                        Done = true;
                        break;
                    }
                case SDL_KEYDOWN:
                    {
                        switch (Event.key.keysym.sym)
                        {
                            case SDLK_1:
                            {
                                Number = 1;
                                break;
                            }
                        case SDLK_2:
                            {
                                Number = 2;
                                break;
                            }
                        case SDLK_3:
                            {
                                Number = 3;
                                break;
                            }
                        case SDLK_4:
                            {
                                Number = 4;
                                break;
                            }
                        case SDLK_5:
                            {
                                Number = 4;
                                break;
                            }
                        case SDLK_6:
                            {
                                Number = 4;
                                break;
                            }
                        case SDLK_7:
                            {
                                Number = 4;
                                break;
                            }
                        case SDLK_SPACE:
                                GenMap(Map);
                                break;
                        }
                    }
                case SDL_MOUSEBUTTONDOWN:
                    {
                        if (Event.button.button = SDL_BUTTON_LEFT) {
                                //Координаты 1 клика
                                int x = Event.button.x;
                                int y = Event.button.y;
                                if (x < 2 * 100 && y < 2 * 100 && x1 == -1) { //Проверка на координаты
                                    x1 = Event.button.x;
                                    y1 = Event.button.y;
                                } else {
                                    //Координаты 2 клика
                                    if (x < 2 * 100 && y < 2 * 100 && x2 == -1) { //Проверка на координаты
                                        x2 = Event.button.x;
                                        y2 = Event.button.y;
                                        cout << "x1 = " << x1;
                                        cout << "y1 = " << y1 << endl;
                                        cout << "Number = " << Number << endl;
                                        cout << "x2 = " << x2;
                                        cout << "y2 = " << y2 << endl;
                                        cout << endl;
                                        FillSpace(Map, Number, x1/2, y1/2, x2/2, y2/2);
                                        x1 = -1, y1 = -1, x2 = -1, y2 = -1;
                                    }
                                }
                        }
                        break;
                    }
            }
        }
        MoveMap(Map);
        DrawMap(Map);
    }
    quit();
    return 0;
}
