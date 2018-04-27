#include "editor.h"

buffer *editor_init()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	ESCDELAY = 25;

	buffer *current_buff = new_buffer();
	return current_buff;
}

buffer *new_buffer()
{
	buffer *new_buff;
	new_buff = malloc(sizeof(buffer));
	new_buff->content = gap_buffer_create();
	new_buff->lines = NULL;
	new_buff->next_buffer = NULL;
	getmaxyx(stdscr, new_buff->height, new_buff->width);
	return new_buff;
}

void buffer_destroy(buffer *current_buff)
{
	if(!current_buff)
	{
		return;
	}

	if(current_buff->content != NULL)
	{
		gap_buffer_destroy(current_buff->content);
	}

	if(current_buff->lines != NULL)
	{
		free(current_buff->lines);
	}

	if(current_buff->next_buffer)
	{
		buffer_destroy(current_buff->next_buffer);
	}

	free(current_buff);
}

bool editor_handle_input(buffer *current_buff, int ch)
{
	if (isprint(ch) && ch != 37)
	{
		gap_buffer_insert_char(current_buff->content, ch);
		editor_move_right(current_buff);
	}
	else
	{

		switch(ch)
		{
			case ALT_ESC_OR_ALT:
				nodelay(stdscr, true);
				ch = getch();
				if ( ch == -1)
				{
					return false;
				}
				nodelay(stdscr, false);
				break;
			case KEY_BACKSPACE:
			case ALT_BACKSPACE:
				if(gap_buffer_backspace_char(current_buff->content) == 0)
				{
					mvprintw(current_buff->height, 0, "no more backspace");
				}
				editor_move_left(current_buff);
				break;
			case KEY_ENTER:
			case ALT_ENTER:
				gap_buffer_insert_char(current_buff->content, '\n');
				editor_new_line(current_buff);
				break;
			case ALT_SPACE:
				gap_buffer_insert_char(current_buff->content, ' ');
				editor_move_right(current_buff);
			case KEY_UP:
				editor_move_up(current_buff);
				break;
			case KEY_DOWN:
				editor_move_down(current_buff);
			case KEY_LEFT:
				editor_move_left(current_buff);
				break;
			case KEY_RIGHT:
				editor_move_right(current_buff);
				break;
			default:
				mvprintw(0, 0, "UNKNOWN KEY %s : %d", keyname(ch), ch);
				break;
		}
	}

	return true;
}

void editor_render(buffer *current_buff)
{
	erase();

	char *current_file = gap_buffer_render(current_buff->content);
	mvprintw(0, 0, "%s", current_file);
	free(current_file);
	move(current_buff->cursor.y, current_buff->cursor.x);
	refresh();
}

void editor_fini(buffer *current_buff)
{
	ESCDELAY = 1000;
	keypad(stdscr, FALSE);
	echo();
	nocbreak();
	endwin();

	char *current_file = gap_buffer_render(current_buff->content);
	printf("FINAL : \"%s\"\n", current_file);
	free(current_file);
	buffer_destroy(current_buff);
}

void editor_move_up(buffer *current_buff)
{
	int newY = current_buff->cursor.y - 1;
	int newX = current_buff->cursor.x;
	editor_update_cursor(current_buff, newY, newX);
}

void editor_move_down(buffer *current_buff)
{
	int newY = current_buff->cursor.y + 1;
	int newX = current_buff->cursor.x;
	editor_update_cursor(current_buff, newY, newX);
}

void editor_move_left(buffer *current_buff)
{
	int newY = current_buff->cursor.y;
	int newX = current_buff->cursor.x - 1;
	editor_update_cursor(current_buff, newY, newX);
}

void editor_move_right(buffer *current_buff)
{
	int newY = current_buff->cursor.y;
	int newX = current_buff->cursor.x + 1;
	editor_update_cursor(current_buff, newY, newX);
}

void editor_new_line(buffer *current_buff)
{
	int newY = current_buff->cursor.y + 1;
	int newX = 0;
	current_buff->num_lines++;
	editor_update_cursor(current_buff, newY, newX);
}

void editor_update_cursor(buffer *current_buff, int newY, int newX)
{
	if (newX > current_buff->width)
	{
		newX = 0;
		newY++;
	}
	else if (newX < 0)
	{
		newX = 0;
		newY--;
	}

	if (newY > current_buff->height)
	{
		newY = current_buff->height;
	}
	else if (newY < 0)
	{
		newY = 0;
	}

	current_buff->cursor.x = newX;
	current_buff->cursor.y = newY;
}
