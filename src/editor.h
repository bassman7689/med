#ifndef SCREEN_H
#define SCREEN_H

#include <assert.h>
#include <ncurses.h>
#include <stdbool.h>
#include "gap_buffer.h"
#include "keyboard.h"

typedef struct position
{
	int x;
	int y;
} position;

typedef struct line
{
	int len;
} line;

typedef struct buffer
{
	struct buffer *next_buffer;
	int width;
	int height;
	position cursor;
	gap_buffer *content;
	int num_lines;
	line *lines;
} buffer;

buffer *editor_init();
buffer *new_buffer();
bool editor_handle_input(buffer *current_buff, int ch);
void editor_render(buffer *current_buff);
void editor_move_up(buffer *current_buff);
void editor_move_down(buffer *current_buff);
void editor_move_left(buffer *current_buff);
void editor_move_right(buffer *current_buff);
void editor_new_line(buffer *current_buff);
void editor_update_cursor(buffer *current_buff, int newY, int newX);
void editor_fini();

#endif
