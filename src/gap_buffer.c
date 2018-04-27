#include "gap_buffer.h"
#include "assert.h"

gap_buffer *gap_buffer_create()
{
	gap_buffer *gb = malloc(sizeof(gap_buffer));
	gb->size = DEFAULT_GAP_SIZE;
	gb->buffer = malloc(sizeof(char) * gb->size);
	gb->gap_start = gb->buffer;
	gb->gap_end = (gb->gap_start + gb->size);
	gb->buffer_end = gb->gap_end;
	return gb;
}

static void gap_buffer_expand(gap_buffer *gb)
{
	int new_size = gb->size * 2;
	char *new_buffer = malloc(sizeof(char) * new_size);

	int before_len = gb->gap_start - gb->buffer;
	memmove(new_buffer, gb->buffer, before_len);

	int after_len = (gb->buffer_end - gb->gap_end);
	memmove(new_buffer + (new_size - after_len), gb->gap_end, after_len);

	gb->gap_start = new_buffer + before_len;
	gb->gap_end = new_buffer + new_size - after_len;

	free(gb->buffer);
	gb->buffer = new_buffer;
	gb->buffer_end = new_buffer + new_size;
	gb->size = new_size;
}

void gap_buffer_insert_char(gap_buffer *gb, char c)
{
	*gb->gap_start = c;
	gb->gap_start++;
	if (gb->gap_start == gb->gap_end)
	{
		gap_buffer_expand(gb);
	};
}

bool gap_buffer_backspace_char(gap_buffer *gb)
{
	if (gb->gap_start == gb->buffer)
	{
		return false;
	}

	gb->gap_start--;
	return true;
}

bool gap_buffer_delete_char(gap_buffer *gb)
{
	if (gb->gap_end == gb->buffer + gb->size)
	{
		return false;
	}

	gb->gap_end++;
	return true;
}

bool gap_buffer_move_gap(gap_buffer *gb, int i)
{
	if (i == 0)
	{
		return false;
	}

	if (i > 0)
	{
		memmove(gb->gap_start, gb->gap_end, i);
		gb->gap_start += i;
		gb->gap_end += i;
		return true;
	}
	else 
	{
                memmove(gb->gap_end + i, gb->gap_start + i, abs(i));
		gb->gap_start += i;
		gb->gap_end += i;
		return true;
	}	
}

void gap_buffer_print(gap_buffer *gb)
{
	char *start;
	for(start = gb->buffer; start < gb->gap_start; start++)
	{
		putchar(*start);
	}

	long numX;
	for(numX = gb->gap_end - gb->gap_start; numX > 0; numX--)
	{
		putchar('x');
	}

	for(start = gb->gap_end; start < (gb->buffer + gb->size); start++)
	{
		putchar(*start);
	}
	putchar('\n');
}

char *gap_buffer_render(gap_buffer *gb)
{
	char *ret;
	int ret_size = (gb->size - (gb->gap_end - gb->gap_start) + 1);
	ret = malloc(sizeof(char) * ret_size);
	memmove(ret, gb->buffer, (gb->gap_start - gb->buffer));

	int after_len = (gb->buffer_end - gb->gap_end);
	memmove(ret + (gb->size - after_len), gb->gap_end, after_len);

	ret[ret_size-1] = '\0';
	return ret;
}

bool gap_buffer_destroy(gap_buffer *gb)
{
	if (gb == NULL)
       	{
		return false;
	}

	if (gb->buffer == NULL)
	{
		free(gb);
		return false;
	}

	free(gb->buffer);
	free(gb);

	return true;
}
