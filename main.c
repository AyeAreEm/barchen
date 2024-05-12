#include "include/raylib.h"
#include "include/dynstr.h"
#include <direct.h>
#include <stdio.h>
#include <string.h>

typedef enum Umlaut {
    Umlaut_Small_ah = -28,
    Umlaut_Small_uh = -4,
} Umlaut;

typedef struct Textbox {
    Rectangle box;
    string *content;
    bool *active;
} Textbox ;

typedef struct State {
    Textbox input;
    Textbox output;
    Rectangle gen_button;
    int width;
    int height;
} State;

void copy_to_clip() {
    char *command = "python3 copy_translation.py";
    system(command);
}

char *translate(string str) {
    char *command = "python3 translator.py";
    char *cstr = string_to_cstr(str);

    int buffer_size = strlen(command) + str.len + 4;
    char *input = malloc(buffer_size);
    snprintf(input, buffer_size, "%s \"%s\"", command, cstr);
    system(input);

    free(input);
    free(cstr);

    FILE *file;
    errno_t err;
    if ((err = fopen_s(&file, "gen.txt", "r")) != 0) {
        printf("couldn't open file");
        exit(1);
    }

    string content = string_new();
    char c;
    while ((c = fgetc(file)) != EOF) {
        printf("num: %d\n", c);

        switch (c) {
            case Umlaut_Small_ah: {
                string tmp = string_from("ä");
                string_pushstr(&content, &tmp);
                free(tmp.data);
                string_print(content);
                continue;        
            }
            case Umlaut_Small_uh: {
                string tmp = string_from("ü");
                string_pushstr(&content, &tmp);
                free(tmp.data);
                string_print(content);
                continue;
            }
            default: {
                string_push(&content, c);
                string_print(content);
            }
        }
    }
    fclose(file);

    char *content_cstr = string_to_cstr(content);
    free(content.data);
    return content_cstr;
}

void update(State *state) {
    Vector2 mouse_pos = GetMousePosition();

    if (CheckCollisionPointRec(mouse_pos, state->gen_button)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *state->input.active = false;
            *state->output.active = false;


            string_clear(state->output.content);
            char *content = translate(*state->input.content);
            for (size_t i = 0; i < strlen(content); i++) {
                string_push(state->output.content, content[i]);
            }
            free(content);
        }
    } else if (CheckCollisionPointRec(mouse_pos, state->input.box)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *state->input.active = true;
            *state->output.active = false;
        }
    } else if (CheckCollisionPointRec(mouse_pos, state->output.box)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *state->output.active = true;
            *state->input.active = false;
        }
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *state->input.active = false;
            *state->output.active = false;
        }
    }

    if (*state->input.active) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125) {
                string_push(state->input.content, key);
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && state->input.content->len > 0) {
            string_pop(state->input.content);
        }
    } else if (*state->output.active) {
        copy_to_clip();
        *state->output.active = false;
    }
}

void draw(State state) {
    char *input_cstr = string_to_cstr(*state.input.content);
    char *output_cstr = string_to_cstr(*state.output.content);
    // printf("output: %s\n", output_cstr);
    BeginDrawing();

    ClearBackground((Color){28, 27, 25, 255 });
    // title
    {
        Rectangle title_rec = {
            .x = ((float)state.width / 2 - 250),
            .y = 20,
            .width = 500,
            .height = 50
        };

        DrawRectangleRoundedLines(title_rec, 0.4, 30, 3, WHITE);
        DrawText("bärchen", state.width / 2 - 50, 30, 25, WHITE);
    }
    // textboxes
    {
        DrawRectangleRounded(state.input.box, 0.1, 30, (Color){50, 50, 50, 255});
        DrawText(input_cstr, state.input.box.x, state.input.box.y, 25, WHITE);
        DrawRectangleRounded(state.output.box, 0.1, 30, (Color){50, 50, 50, 255});
        DrawText(output_cstr, state.output.box.x, state.output.box.y, 25.0, WHITE);
    }
    // generate button
    {
        DrawRectangleRounded(state.gen_button, 0.1, 30, BLACK);
        DrawText("->", state.gen_button.x + (state.gen_button.width / 2) - 10, state.gen_button.y + (state.gen_button.height / 2) - 10, 20, WHITE);
    }

    EndDrawing();
    free(input_cstr);
    free(output_cstr);
}

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 840, "bärchen");
    SetTargetFPS(60);

    string input_con = string_new();
    bool input_active = false;
    string output_con = string_new();
    bool output_active = false;

    while (!WindowShouldClose()) {
        int width = GetScreenWidth();
        int height = GetScreenHeight();

        Rectangle input_rec = {
            .x = 20,
            .y = 100,
            .width = (float)width / 2.3,
            .height = (float)height - 150
        };

        Rectangle output_rec = {
            .x = (float)width - 20 - ((float)width / 2.3),
            .y = 100,
            .width = (float)width / 2.3,
            .height = (float)height - 150
        };

        Rectangle gen_button = {
            .x = (float)width / 2 - 50,
            .y = ((float)height - 150) / 2,
            .width = 100,
            .height = 70,
        };

        State state = {
            .input = {
                .box = input_rec,
                .content = &input_con,
                .active = &input_active,
            },
            .output = {
                .box = output_rec,
                .content = &output_con,
                .active = &output_active,
            },
            .gen_button = gen_button,
            .width = width,
            .height = height
        };

        update(&state);
        draw(state);
    }

    free(input_con.data);
    free(output_con.data);

    CloseWindow();
}
