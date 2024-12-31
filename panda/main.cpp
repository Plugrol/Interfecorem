#include "main.h"
#include "Game.h"

int main() 
{

	Game game(GameInfo
	{
		new char[] {"Unfairtrade Games Studio (Project Panda)"},
		VK_MAKE_VERSION(0, 0, 1),
		VkExtent2D { 800, 600 }
	});

	//Don't know if I have to something between here. Just seperated them.

	game.start();

	return 0;
	
}
