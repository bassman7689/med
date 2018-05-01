#ifndef __GAP_BUFFER
#define __GAP_BUFFER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int cursor;

typedef struct buffer_t {
    char* data;
    cursor gap_start;
    cursor gap_end;
    unsigned int size;
    cursor point;
} buffer;

buffer* new_buffer(unsigned int size);
void free_buffer(buffer *b);
cursor buffer_insert_at_cursor(buffer* b, char character, cursor position);
cursor buffer_delete_backward(buffer* b, cursor position);
cursor buffer_delete_forward(buffer* b, cursor position);
cursor buffer_newline(buffer* b, cursor position);
void buffer_debug_print(buffer *b);
char *buffer_render(buffer *b);

cursor increment_cursor(buffer *b, cursor position);
cursor decrement_cursor(buffer *b, cursor position);
cursor move_cursor_down_a_line(buffer *b, cursor position);
void cursor_to_screen_coordinates(buffer *b, cursor position, int *x, int *y);
#endif
