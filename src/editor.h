#ifndef SCREEN_H
#define SCREEN_H

#include <assert.h>
#include <ncurses.h>
#include <stdbool.h>

#include "gap_buffer.h"

#include "keyboard.h"

typedef uint32_t Cursor;

gap_buffer *editor_init();
gap_buffer *new_buffer();
bool editor_handle_input(gap_buffer *current_buff, int ch);
void editor_render(gap_buffer *current_buff);
void editor_fini();

#endif
