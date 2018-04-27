#include "gap_buffer.h"

gap_buffer* new_gap_buffer(unsigned int size) {
    gap_buffer* gb = malloc(sizeof(gap_buffer));
    gb->buffer = malloc(sizeof(char) * size);
    gb->gap_start = 0;
    gb->gap_end = size;
    gb->size = size;
    gb->point = 0;
    return gb;
}

void free_gap_buffer(gap_buffer* gb) {
    free(gb->buffer);
    free(gb);
}

static void gap_buffer_grow(gap_buffer *gb) {
    unsigned int new_size = gb->size * 2;

    char * new_buffer = malloc(sizeof(char) * new_size);
    memmove(new_buffer, gb->buffer, gb->gap_start);

    unsigned int new_gap_end = new_size - (gb->size - gb->gap_end);
    memmove(new_buffer + new_gap_end, gb->buffer + gb->gap_end, gb->size - gb->gap_end);

    free(gb->buffer);

    gb->buffer = new_buffer;
    gb->gap_end = new_gap_end;
    gb->size = gb->size * 2;
}

cursor increment_cursor(gap_buffer *gb, cursor position) {
    if(position >= gb->size) {
        return gb->size;
    }

    position++;
    if(position > gb->gap_start) {
        position = gb->gap_end;
    }

    return position;
}

cursor decrement_cursor(gap_buffer *gb, cursor position) {
    if (position <= 0) {
        return 0;
    }

    position--;
    if (gb->gap_start < position && gb->gap_end > position) {
        position = gb->gap_start;
    }

    return position;
}

static void gap_buffer_move_gap(gap_buffer *gb, cursor position) {
    if (position < gb->gap_start) {
        unsigned int gap_delta = gb->gap_start - position;

        gb->gap_start -= gap_delta;
        gb->gap_end -= gap_delta;

        memmove(gb->buffer + gb->gap_end, gb->buffer + gb->gap_start, gap_delta);
    } else if (position > gb->gap_start) {
        unsigned int gap_delta = position - gb->gap_start;

        memmove(gb->buffer + gb->gap_start, gb->buffer + gb->gap_end, gap_delta);

        gb->gap_start += gap_delta;
        gb->gap_end += gap_delta;
    }
}

cursor gap_buffer_insert_at_cursor(gap_buffer* gb, char character, cursor position) {
    gap_buffer_move_gap(gb, position);

    gb->buffer[gb->gap_start] = character;
    gb->gap_start++;
    position++;

    if (gb->gap_start == gb->gap_end) {
        gap_buffer_grow(gb);
    }

    return position;
}

cursor gap_buffer_delete_back(gap_buffer* gb, cursor position) {
    gap_buffer_move_gap(gb, position);
    if(gb->gap_start > 0) {
        gb->gap_start--;
    }
    position--;
    return position;
}

cursor gap_buffer_delete_forward(gap_buffer* gb, cursor position) {
    gap_buffer_move_gap(gb, position);
    if (gb->gap_end < gb->size) {
        gb->gap_end++;
    }
    return position;
}

cursor gap_buffer_newline(gap_buffer *gb, cursor position) {
    return gap_buffer_insert_at_cursor(gb, '\n', position);
}

char *gap_buffer_render(gap_buffer *gb) {
    int size = gb->size - gb->gap_end + gb->gap_start;
    char *data = malloc(sizeof(char) * size + 1);
    memmove(data, gb->buffer, gb->gap_start);
    memmove(data + gb->gap_start, gb->buffer + gb->gap_end, gb->size - gb->gap_end);
    data[size] = '\0';
    return data;
}


void gap_buffer_debug_print(gap_buffer *gb) {
    unsigned int i;

    for(i = 0; i < gb->gap_start; i++) {
        printf("%c", gb->buffer[i]);
    }

    for (i = gb->gap_start; i < gb->gap_end; i++) {
        printf("-");
    }

    for (i = gb->gap_end; i < gb->size; i++) {
        printf("%c", gb->buffer[i]);
    }

    printf("\n");
}

