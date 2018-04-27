#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_GAP_SIZE 8

typedef struct gap_buffer
{
	size_t size;
	char *buffer;
	char *gap_start;
	char *gap_end;
	char *buffer_end;
} gap_buffer;

gap_buffer *gap_buffer_create();

void gap_buffer_insert_char(gap_buffer *gb, char c);

bool gap_buffer_backspace_char(gap_buffer *gb);

bool gap_buffer_delete_char(gap_buffer *gb);

bool gap_buffer_move_gap(gap_buffer *gb, int i);

void gap_buffer_print(gap_buffer *gb);

char *gap_buffer_render(gap_buffer *gb);

bool gap_buffer_destroy(gap_buffer *gb);

#endif
