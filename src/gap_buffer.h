#ifndef __GAP_BUFFER
#define __GAP_BUFFER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int cursor;

typedef struct gap_buffer_t {
    char* buffer;
    unsigned int gap_start;
    unsigned int gap_end;
    unsigned int size;
    cursor point;
} gap_buffer;

gap_buffer* new_gap_buffer(unsigned int size);
void free_gap_buffer(gap_buffer *gb);
cursor gap_buffer_insert_at_cursor(gap_buffer* gb, char character, cursor position);
cursor gap_buffer_delete_back(gap_buffer* gb, cursor position);
cursor gap_buffer_delete_forward(gap_buffer* gb, cursor position);
cursor gap_buffer_newline(gap_buffer* gb, cursor position);
void gap_buffer_debug_print(gap_buffer *gb);
char *gap_buffer_render(gap_buffer *gb);

cursor increment_cursor(gap_buffer *gb, cursor position);
cursor decrement_cursor(gap_buffer *gb, cursor position);
#endif
