#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "cutils.h"
#include "sudokulib.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_TextEngine* text_engine = NULL;
static TTF_Font* number_font = NULL;
static SDL_Event event;
static input_mode_t input_mode = NUMBER;
static sudoku_board_t board = { 0 };

static bool quitting = false;
static SDL_Color BACKGROUND_COLOR = { 100, 0, 0, SDL_ALPHA_OPAQUE };
static float CELL_WIDTH = 50.0;


int initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        printf("Could not initialize: %s\n", SDL_GetError());
        return 1;
    }
    if (!SDL_CreateWindowAndRenderer("SDL Sudoku", 800, 600, 0, &window, &renderer)) {
        printf("Could not create window and renderer: %s\n", SDL_GetError());
        return 1;
    }
    if (!TTF_Init()) {
        printf("Could not initialize TTF-library: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}

void update() {
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            quitting = true;
            break;

        case SDL_EVENT_KEY_DOWN: {
            switch (input_mode) {
            case NUMBER: {
                switch (event.key.key)
                {
                case SDLK_ESCAPE:
                    quitting = true;
                    break;
                case SDLK_H:
                    printf("Pressed the H-key!\n");
                    input_mode = HINT;
                    break;
                case SDLK_LEFT:
                    sudoku_move_cursor(&board, LEFT);
                    break;
                case SDLK_DOWN:
                    sudoku_move_cursor(&board, DOWN);
                    break;
                case SDLK_RIGHT:
                    sudoku_move_cursor(&board, RIGHT);
                    break;
                case SDLK_UP:
                    sudoku_move_cursor(&board, UP);
                    break;
                case SDLK_0:
                    update_number(&board, 0);
                    break;
                case SDLK_1:
                    update_number(&board, 1);
                    break;
                case SDLK_2:
                    update_number(&board, 2);
                    break;
                case SDLK_3:
                    update_number(&board, 3);
                    break;
                case SDLK_4:
                    update_number(&board, 4);
                    break;
                case SDLK_5:
                    update_number(&board, 5);
                    break;
                case SDLK_6:
                    update_number(&board, 6);
                    break;
                case SDLK_7:
                    update_number(&board, 7);
                    break;
                case SDLK_8:
                    update_number(&board, 8);
                    break;
                case SDLK_9:
                    update_number(&board, 9);
                    break;
                default:
                    break;
                }
                break;
            }
            case HINT: {
                int key_pressed = event.key.key - 48;
                if (key_pressed >= 0 && key_pressed <= 9) { 
                    update_hint(&board, key_pressed);
                }
                input_mode = NUMBER;
                break;
            }
            }
            
            break;
        }
        default:
            break;
        }
    }
}


int main(int argc, char const* argv[])
{
    int window_width, window_height;
    float fps = 0;
    uint64_t old_ticks = 0;
    uint64_t new_ticks = 0;
    char *fps_string = calloc(50, 1);
    if (fps_string == NULL) return 1;
    int initialized = initialize();

    if (initialized != 0) {
        return initialized;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    text_engine = TTF_CreateRendererTextEngine(renderer);
    number_font = TTF_OpenFont("fonts/DejaVuSansMono.ttf", 50);
    if (number_font == NULL) {
        printf("Could not load font: %s\n", SDL_GetError());
    }

    size_t filesize;
    string_t sudoku_string = { (char*)SDL_LoadFile("sudokuboard.txt", &filesize), filesize };
    printf("%s, size: %d\n", sudoku_string.data, sudoku_string.length);

    load_board_from_string(&board, &sudoku_string);
    board.cells[0][0].hints[0] = 1;
    board.cells[0][0].hints[1] = 2;
    board.cells[0][0].hints[2] = 3;
    board.cells[0][0].hints[3] = 4;
    board.cells[0][0].hints[4] = 5;
    board.cells[0][0].hints[5] = 6;
    board.cells[0][0].hints[6] = 7;
    board.cells[0][0].hints[7] = 8;
    board.cells[0][0].hints[8] = 9;
    board.cells[0][2].number = 8;
    board.cells[8][6].number = 4;
    board.cells[7][3].number = 5;
    validate_board(&board);
    print_board(&board, true);

    while (!quitting)
    {
        new_ticks = SDL_GetTicksNS();
        SDL_GetWindowSize(window, &window_width, &window_height);
        update();
        if (quitting) break;

        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
        SDL_RenderClear(renderer);
        render_sudoku_board(renderer, text_engine, number_font, &board, 100.0, 100.0, CELL_WIDTH, input_mode);
        sprintf_s(fps_string, 50, "FPS: %.2f", 1000000000.0 / (new_ticks - old_ticks));
        SDL_RenderDebugText(renderer, (float)20, (float)window_height - 20, fps_string);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
        old_ticks = new_ticks;
    }

    TTF_CloseFont(number_font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_free(sudoku_string.data);
    SDL_Quit();

    printf("Test!\n");
    return 0;
}
