#ifndef SUDOKU_LIB_C
#define SUDOKU_LIB_C

#include "sudokulib.h"

void sudoku_move_cursor(sudoku_board_t* board, direction_t direction) {
    if (board == NULL) return;
    switch (direction) {
    case UP:
        if (board->selected_row > 0) board->selected_row--;
        break;
    case RIGHT:
        if (board->selected_col < 8) board->selected_col++;
        break;
    case DOWN:
        if (board->selected_row < 8) board->selected_row++;
        break;
    case LEFT:
        if (board->selected_col > 0) board->selected_col--;
        break;
    default:
        break;
    }
}

void update_hint(sudoku_board_t* board, int number) {
    if (number < 0 || number > 9) return;
    cell_t* selected_cell = &(board->cells[board->selected_row][board->selected_col]);
    if (!selected_cell->editable) return;
    if (selected_cell->number != 0) return;
    if (number == 0) {
        for (int i = 0; i < 9; i++) {
            selected_cell->hints[i] = 0;
        }
        return;
    }
    if (selected_cell->hints[number-1] == 0) {
        selected_cell->hints[number-1] = number;
    }
    else {
        selected_cell->hints[number-1] = 0;
    }
}

void update_number(sudoku_board_t* board, int number) {
    if (number < 0 || number > 9) return;
    if (!board->cells[board->selected_row][board->selected_col].editable) return;
    board->cells[board->selected_row][board->selected_col].number = number;
}

void validate_board(sudoku_board_t* board) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            board->cells[row][col].valid = true;
        }
    }
    //Check rows
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            cell_t* num1 = &board->cells[row][col];
            if (num1->number == 0) continue;
            if (!num1->editable) continue;

            for (int k = col + 1; k < 9; k++) {
                cell_t* num2 = &board->cells[row][k];
                if (num2->number == num1->number) {
                    num1->valid = false;
                }
            }
        }
    }
    //Check columns
    for (int col = 0; col < 9; col++) {
        for (int row = 0; row < 9; row++) {
            cell_t* num1 = &board->cells[row][col];
            if (num1->number == 0) continue;
            if (!num1->editable) continue;

            for (int k = row + 1; k < 9; k++) {
                cell_t* num2 = &board->cells[k][col];
                if (num1->number == num2->number) {
                    num1->valid = false;
                }
            }
        }
    }
    //Check squares - this is horrible. There definitely exists a much better way of doing this...
    for (int row_square = 0; row_square < 9; row_square += 3) {
        for (int col_square = 0; col_square < 9; col_square += 3) {
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    cell_t* num1 = &board->cells[row_square + row][col_square + col];
                    if (num1->number == 0) continue;
                    if (!num1->editable) continue;
                    for (int k_row = 0; k_row < 3; k_row++) {
                        for (int k_col = 0; k_col < 3; k_col++) {
                            cell_t* num2 = &board->cells[row_square + k_row][col_square + k_col];

                            if (num1 == num2) continue;
                            if (num1->number == num2->number) {
                                num1->valid = false;
                            }
                        }
                    }
                }
            }
        }
    }
}

void load_board_from_string(sudoku_board_t* board, string_t string) {
    if (string.length != 81) return;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            board->cells[row][col].number = string.data[row * 9 + col] - 48;
            board->cells[row][col].valid = true;
            if (board->cells[row][col].number != 0) {
                board->cells[row][col].editable = false;
            }
            else {
                board->cells[row][col].editable = true;
            }
        }
    }
}

void print_board(sudoku_board_t* board, bool print_valid) {
    printf("\n");
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            printf("%d", board->cells[row][col].number);
        }
        printf("\n");
    }
    printf("\n");
    if (!print_valid) return;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board->cells[row][col].valid) {
                printf("v");
            }
            else {
                printf("F");
            }
        }
        printf("\n");
    }
}

void render_sudoku_board(SDL_Renderer* renderer, TTF_TextEngine* text_engine, TTF_Font* font, sudoku_board_t* board, float x, float y, float width, input_mode_t input_mode) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            SDL_FRect rect = { x + col * width, y + row * width, width, width };
            if (!board->cells[row][col].valid) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 50, SDL_ALPHA_OPAQUE);
            }
            else if (board->cells[row][col].editable) {
                SDL_SetRenderDrawColor(renderer, 240, 240, 240, SDL_ALPHA_OPAQUE);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 120, 120, 120, SDL_ALPHA_OPAQUE);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    if (board->selected_row < 9 && board->selected_col < 9) {
        SDL_FRect selected_rect = { x + board->selected_col * width, y + board->selected_row * width, width, width };
        SDL_SetRenderDrawColor(renderer, 100, 100, 150, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &selected_rect);
    }
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, x + width * 3, y, x + width * 3, y + width * 9);
    SDL_RenderLine(renderer, x + width * 6, y, x + width * 6, y + width * 9);
    SDL_RenderLine(renderer, x, y + width * 3, x + width * 9, y + width * 3);
    SDL_RenderLine(renderer, x, y + width * 6, x + width * 9, y + width * 6);

    SDL_Surface* num_surfs[10];
    SDL_Texture* num_textures[10];

    num_surfs[0] = TTF_RenderGlyph_Blended(font, '_', (SDL_Color) { 0, 0, 0, SDL_ALPHA_OPAQUE });
    num_textures[0] = SDL_CreateTextureFromSurface(renderer, num_surfs[0]);

    for (int i = 1; i < 10; i++) {
        num_surfs[i] = TTF_RenderGlyph_Blended(font, i + 48, (SDL_Color) { 0, 0, 0, SDL_ALPHA_OPAQUE });
        num_textures[i] = SDL_CreateTextureFromSurface(renderer, num_surfs[i]);
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board->cells[row][col].number == 0) {
                SDL_SetRenderDrawColor(renderer, 120, 120, 120, SDL_ALPHA_OPAQUE);
                int num_hints = 0;
                for (int i = 0; i < 9; i++) {
                    if (board->cells[row][col].hints[i] == 0) continue;
                    num_hints++;
                    SDL_FRect hint_dest = {x + width * col, y + width * row, num_surfs[i+1]->w / 3.0f, num_surfs[i+1]->h / 3.0f};
                    switch (num_hints) {
                    case 1:
                        break;
                    case 2:
                        hint_dest.x += width / 3;
                        break;
                    case 3:
                        hint_dest.x += (width / 3) * 2;
                        break;
                    case 4:
                        hint_dest.y += width / 3;
                        break;
                    case 5:
                        hint_dest.y += width / 3;
                        hint_dest.x += (width / 3) * 2;
                        break;
                    case 6:
                        hint_dest.y += (width / 3) * 2;
                        break;
                    case 7:
                        hint_dest.y += (width / 3) * 2;
                        hint_dest.x += (width / 3);
                        break;
                    case 8:
                        hint_dest.y += (width / 3) * 2;
                        hint_dest.x += (width / 3) * 2;
                        break;
                    case 9:
                        hint_dest.y += width / 3;
                        hint_dest.x += width / 3;
                        break;
                    default:
                        break;
                    }
                    SDL_RenderTexture(renderer, num_textures[i+1], NULL, &hint_dest);
                }
                if (input_mode == HINT && row == board->selected_row && col == board->selected_col && num_hints < 9) {
                    num_hints++;
                    SDL_FRect underscore_dest = { x + width * col, y + width * row, num_surfs[0]->w / 3.0f, num_surfs[0]->h / 3.0f };
                    switch (num_hints) {
                    case 1:
                        break;
                    case 2:
                        underscore_dest.x += width / 3;
                        break;
                    case 3:
                        underscore_dest.x += (width / 3) * 2;
                        break;
                    case 4:
                        underscore_dest.y += width / 3;
                        break;
                    case 5:
                        underscore_dest.y += width / 3;
                        underscore_dest.x += (width / 3) * 2;
                        break;
                    case 6:
                        underscore_dest.y += (width / 3) * 2;
                        break;
                    case 7:
                        underscore_dest.y += (width / 3) * 2;
                        underscore_dest.x += (width / 3);
                        break;
                    case 8:
                        underscore_dest.y += (width / 3) * 2;
                        underscore_dest.x += (width / 3) * 2;
                        break;
                    case 9:
                        underscore_dest.y += width / 3;
                        underscore_dest.x += width / 3;
                        break;
                    default:
                        break;
                    }
                    SDL_RenderTexture(renderer, num_textures[0], NULL, &underscore_dest);
                }
                continue;
            }
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE);
            SDL_FRect main_dest = { x + width * col + width * 0.25f, y + width * row, width / 2, width };
            SDL_RenderTexture(renderer, num_textures[board->cells[row][col].number], NULL, &main_dest);
        }
    }
    for (int i = 0; i < 10; i++) {
        SDL_DestroySurface(num_surfs[i]);
        SDL_DestroyTexture(num_textures[i]);
    }

}

#endif