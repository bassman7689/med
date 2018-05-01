#include "editor.h"

WINDOW* win;

buffer *editor_init() {
	win = initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	ESCDELAY = 25;

	buffer *bufer = new_buffer(2);
	return bufer;
}

bool editor_handle_input(buffer *buf, int ch)
{
	if (isprint(ch) && (ch != ALT_SPACE ||
            ch != KEY_ENTER ||
            ch != ALT_ENTER))
	{
        buf->point = buffer_insert_at_cursor(buf, ch, buf->point);
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
				buf->point = buffer_delete_backward(buf, buf->point);
				break;
			case KEY_ENTER:
			case ALT_ENTER:
				buffer_insert_at_cursor(buf, '\n', buf->point);
				break;
			case ALT_SPACE:
				buffer_insert_at_cursor(buf, ' ', buf->point);
			case KEY_UP:
				break;
			case KEY_DOWN:
                fprintf(stderr, "buf->point = %d", buf->point);
                buf->point = move_cursor_down_a_line(buf, buf->point);
                fprintf(stderr, "buf->point = %d", buf->point);
                break;
			case KEY_LEFT:
				buf->point = decrement_cursor(buf, buf->point);
				break;
			case KEY_RIGHT:
				buf->point = increment_cursor(buf, buf->point);
				break;
			default:
				mvprintw(0, 0, "UNKNOWN KEY %s : %d", keyname(ch), ch);
				break;
		}
	}

	return true;
}

void editor_render(buffer *buf)
{
	erase();

	char *current_file = buffer_render(buf);
	mvprintw(0, 0, "%s", current_file);
    int x, y;
    cursor_to_screen_coordinates(buf, buf->point, &x, &y);
    //fprintf(stderr, "buf->point = %d, x = %d, y = %d\n", buf->point, x, y);
    wmove(win, y, x);
	free(current_file);
	refresh();
}

void editor_fini(buffer *buf)
{
	ESCDELAY = 1000;
	keypad(stdscr, FALSE);
	echo();
	nocbreak();
	endwin();

	char *current_file = buffer_render(buf);
	printf("FINAL: \"%s\"\n", current_file);
	free(current_file);
	free_buffer(buf);
}
