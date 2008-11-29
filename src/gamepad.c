#include <stdio.h>
#include <SDL/SDL.h>

#define K_UP 0
#define K_DN 1
#define K_LT 2
#define K_RT 3
#define K_SP 4

static int joy_held[10];

static void hold (int key_held[], int k, int held)
{
	if (held) {
		joy_held[k] = 1;
		key_held[k] = 1;
	}
	else {
		if (joy_held[k])
			key_held[k] = 0;
		joy_held[k] = 0;
	}
}

void HandleGamepad (int key_held[])
{
	static char state = '0';
#define MAX 32
	static int axes[MAX];
	static int buttons[MAX];
	static int num_axes;
	static int num_buttons;
	static SDL_Joystick* joy;

	if (state == '0') {
		state = '?';
		memset (joy_held, 0, sizeof(joy_held));

		if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
			fprintf (stderr, "SDL_init(): %s\n", SDL_GetError());
			return;
		}

		// select joystick
		int num_joysticks = SDL_NumJoysticks();
		int i;
		for (i = 0; i < num_joysticks; i++)
			printf ("joystick %d = \"%s\"\n", i, SDL_JoystickName (i));

		if (num_joysticks != 1) {
			printf ("error: %d joysticks detected\n", num_joysticks);
			return;
		}
		int joystick_index = 0;

		joy = SDL_JoystickOpen (joystick_index);
		if (joy == NULL) {
			fprintf (stderr, "SDL_JoystickOpen(): %s\n", SDL_GetError());
			return;
		}

		printf ("Number of Axes: %d\n", num_axes = SDL_JoystickNumAxes(joy));
		printf ("Number of Buttons: %d\n", num_buttons = SDL_JoystickNumButtons(joy));
		printf ("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
		printf ("Number of Hats: %d\n", SDL_JoystickNumHats(joy));

		state = '1';
	}
	else if (state == '1') {

		SDL_JoystickUpdate();

		int i;
		for (i = 0;  i < num_axes && i < MAX;  ++i)
			axes[i] = SDL_JoystickGetAxis (joy, i);

		for (i = 0;  i < num_buttons && i < MAX;  ++i)
			buttons[i] = SDL_JoystickGetButton (joy, i);

		hold (key_held, K_UP, axes[1] < -1000 || axes[3] < -1000 || buttons[4]);
		hold (key_held, K_RT, axes[0] >  1000 || axes[2] >  1000 || buttons[5]);
		hold (key_held, K_DN, axes[1] >  1000 || axes[3] >  1000 || buttons[6]);
		hold (key_held, K_LT, axes[0] < -1000 || axes[2] < -1000 || buttons[7]);
		hold (key_held, K_SP, buttons[14]);
	}
}

