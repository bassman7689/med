#include <assert.h>

#include "buffer.h"

buffer* new_buffer(unsigned int size) {
    buffer* b = malloc(sizeof(buffer));
    b->data = malloc(sizeof(char) * size);
    b->gap_start = 0;
    b->gap_end = size;
    b->size = size;
    b->point = 0;
    return b;
}

void free_buffer(buffer* b) {
    assert(b);
    assert(b->data);
    free(b->data);
    free(b);
}

static void buffer_grow(buffer *b) {
    unsigned int new_size = b->size * 2;

    char * new_buffer = malloc(sizeof(char) * new_size);
    memmove(new_buffer, b->data, b->gap_start);

    unsigned int new_gap_end = new_size - (b->size - b->gap_end);
    memmove(new_buffer + new_gap_end, b->data + b->gap_end, b->size - b->gap_end);

    free(b->data);

    b->data = new_buffer;
    b->gap_end = new_gap_end;
    b->size = b->size * 2;
}

cursor increment_cursor(buffer *b, cursor position) {
    if(position >= (b->size - 1)) {
        position = b->size-1;
    }

    position++;
    if (position >= b->gap_start && position < b->gap_end) {
        if (b->gap_end == b->size) {
            position = b->gap_start;
        } else {
            position = b->gap_end + 1;
        }
    }

    assert(position <= b->gap_start || position >= b->gap_end);
    return position;
}

cursor decrement_cursor(buffer *b, cursor position) {
    if (position <= 0) {
        return 0;
    }

    position--;
    if (position > b->gap_start && position <= b->gap_end) {
        if(b->gap_start == 0) {
            position = b->gap_end;
        } else {
            position = b->gap_start - 1;
        }
    }

    assert(position <= b->gap_start || position >= b->gap_end);
    return position;
}

static cursor buffer_move_gap(buffer *b, cursor position) {
    if (position < b->gap_start) {
        unsigned int gap_delta = b->gap_start - position;

        int new_start = b->gap_start - gap_delta;
        int new_end = b->gap_end - gap_delta;

        b->gap_start = new_start;
        b->gap_end = new_end;

        assert((new_end - new_start) == (b->gap_end - b->gap_start));
        memmove(b->data + b->gap_end, b->data + b->gap_start, gap_delta);
    } else if (position > b->gap_start) {
        unsigned int gap_delta = position - b->gap_start;

        unsigned int new_start = b->gap_start + gap_delta;
        if (new_start > b->size - (b->gap_end - b->gap_start)) {
            new_start = b->size - (b->gap_end - b->gap_start);
        }

        int new_end = new_start + (b->gap_end - b->gap_start);

        assert((new_end - new_start) == (b->gap_end - b->gap_start));
        memmove(b->data + b->gap_start, b->data + b->gap_end, gap_delta);

        b->gap_start = new_start;
        b->gap_end = new_end;
    }

    position = b->gap_start;
    return position;
}

cursor buffer_insert_at_cursor(buffer* b, char character, cursor position) {
    position = buffer_move_gap(b, position);

    b->data[b->gap_start] = character;
    b->gap_start++;
    position++;

    if (b->gap_start == b->gap_end) {
        buffer_grow(b);
    }

    return position;
}

cursor buffer_delete_backward(buffer* b, cursor position) {
    position = buffer_move_gap(b, position);
    if(b->gap_start > 0) {
        b->gap_start--;
        position--;
    }
    return position;
}

cursor buffer_delete_forward(buffer* b, cursor position) {
    position = buffer_move_gap(b, position);
    if (b->gap_end < b->size) {
        b->gap_end++;
    }
    return position;
}

static cursor end_of_buffer(buffer *b) {
    if (b->gap_end == b->size) {
        return b->gap_end;
    } else {
        return b->size - 1;
    }
}

char buffer_peek(buffer *b, cursor position) {
    return b->data[position];
}

char *buffer_render(buffer *b) {
    int size = b->size - b->gap_end + b->gap_start;
    char *data = malloc(sizeof(char) * size + 1);
    memmove(data, b->data, b->gap_start);
    memmove(data + b->gap_start, b->data + b->gap_end, b->size - b->gap_end);
    data[size] = '\0';
    return data;
}

void cursor_to_screen_coordinates(buffer *b, cursor position, int* x, int* y) {
    *x = *y = 0;
    for(cursor scanner = 0; scanner < position; scanner = increment_cursor(b, scanner)) {
        *x += 1;
        if(buffer_peek(b, scanner) == '\n') {
            *y += 1;
            *x = 0;
        }
    }
}

cursor move_cursor_down_a_line(buffer *b, cursor position) {
    cursor start = position;
    int distance_from_newline = 0;

    while(buffer_peek(b, start) != '\n' && start != 0) {
        start = decrement_cursor(b, start);
        distance_from_newline++;
    }

    start++;
    while(buffer_peek(b, start) != '\n' && start <= end_of_buffer(b)) {
        start++;
    }

    if(start != end_of_buffer(b) && start + distance_from_newline < end_of_buffer(b)) {
        start += distance_from_newline;
    }

    return start;
}

void buffer_debug_print(buffer *b) {
    unsigned int i;

    for(i = 0; i < b->gap_start; i++) {
        printf("%c", b->data[i]);
    }

    for (i = b->gap_start; i < b->gap_end; i++) {
        printf("-");
    }

    for (i = b->gap_end; i < b->size; i++) {
        printf("%c", b->data[i]);
    }

    printf("\n");
}

