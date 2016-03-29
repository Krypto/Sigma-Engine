#include <stdio.h>
#include "Window.h"
#include "GameLoop.h"
#include "GameExample.h"

int main(int argc, char **argv)
{
	sig::Window *window = new sig::Window("Sigma Engine", 286, 512);
	sig::GameLoop *mainLoop = new sig::GameLoop(window);

	GameExample *game = new GameExample();
	mainLoop->SetGame(game);

	return mainLoop->Start();
}
