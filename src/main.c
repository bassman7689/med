#include "editor.h"

int main(int argc, char **argv)
{

	buffer *current_buff = editor_init();

	int ch;
	bool running = true;
	while (running == true)
	{
		ch = getch();
		running = editor_handle_input(current_buff, ch);
		editor_render(current_buff);
	}

	editor_fini(current_buff);

}
