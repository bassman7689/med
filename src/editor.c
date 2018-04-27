#include "editor.h"

gap_buffer *editor_init() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	ESCDELAY = 25;

	gap_buffer *current_buffer = new_gap_buffer(2);
	return current_buffer;
}

bool editor_handle_input(gap_buffer *current_buff, int ch)
{
	if (isprint(ch) && ch != 37)
	{
		current_buff->point = gap_buffer_insert_at_cursor(current_buff, ch, current_buff->point);
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
				current_buff->point = gap_buffer_delete_back(current_buff, current_buff->point);
				break;
			case KEY_ENTER:
			case ALT_ENTER:
				gap_buffer_newline(current_buff, current_buff->point);
				break;
			case ALT_SPACE:
				gap_buffer_insert_at_cursor(current_buff, ' ', current_buff->point);
			case KEY_UP:
				break;
			case KEY_DOWN:
                break;
			case KEY_LEFT:
				current_buff->point = decrement_cursor(current_buff, current_buff->point);
				break;
			case KEY_RIGHT:
				current_buff->point = increment_cursor(current_buff, current_buff->point);
				break;
			default:
				mvprintw(0, 0, "UNKNOWN KEY %s : %d", keyname(ch), ch);
				break;
		}
	}

	return true;
}

void editor_render(gap_buffer *current_buff)
{
	erase();

	char *current_file = gap_buffer_render(current_buff);
	mvprintw(0, 0, "%s", current_file);
	free(current_file);
	refresh();
}

void editor_fini(gap_buffer *current_buff)
{
	ESCDELAY = 1000;
	keypad(stdscr, FALSE);
	echo();
	nocbreak();
	endwin();

	printf("FINAL : \"");
    gap_buffer_debug_print(current_buff);
    printf("\"\n");
	free_gap_buffer(current_buff);
}
