#ifndef SUDOKU_LIB_H
#define SUDOKU_LIB_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "cutils.h"
#include <stdio.h>

typedef enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction_t;

typedef enum InputMode {
    NUMBER,
    HINT
} input_mode_t;

typedef struct Cell {
    int32_t hints[9];
    int32_t number;
    bool editable;
    bool valid;
} cell_t;

typedef struct SudokuBoard {
    cell_t cells[9][9];
    uint8_t selected_row;
    uint8_t selected_col;
} sudoku_board_t;

void validate_board(sudoku_board_t* board);

void load_board_from_string(sudoku_board_t* board, string_t* string);

void print_board(sudoku_board_t* board, bool print_valid);

void render_sudoku_board(SDL_Renderer* renderer, TTF_TextEngine* text_engine, TTF_Font* font, sudoku_board_t* board, float x, float y, float width, input_mode_t input_mode);

void sudoku_move_cursor(sudoku_board_t* board, direction_t direction);

void update_hint(sudoku_board_t* board, int number);

void update_number(sudoku_board_t* board, int number);

#endif
