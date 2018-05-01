#ifndef SCREEN_H
#define SCREEN_H

#include <assert.h>
#include <ncurses.h>
#include <stdbool.h>

#include "buffer.h"

#include "keyboard.h"

typedef uint32_t Cursor;

buffer *editor_init();
buffer *new_buffer();
bool editor_handle_input(buffer *current_buff, int ch);
void editor_render(buffer *current_buff);
void editor_fini();

#endif
