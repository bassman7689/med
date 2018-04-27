#ifndef __GAP_BUFFER
#define __GAP_BUFFER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct gap_buffer_t {
    char* buffer;
    unsigned int gap_start;
    unsigned int gap_end;
    unsigned int size;
} gap_buffer;

gap_buffer* new_gap_buffer(unsigned int size);
void free_gap_buffer(gap_buffer *gb);
void gap_buffer_insert_character_at_position(gap_buffer* gb, char character, unsigned int position);
void gap_buffer_delete_back(gap_buffer* gb, unsigned int position);
void gap_buffer_delete_forward(gap_buffer* gb, unsigned int position);
void gap_buffer_debug_print(gap_buffer *gb);

#endif
