#include "core/Game.h"
#include "scenes/AllScenes.h"
#include "scenes/MainMenu.h"
#include "scenes/level1/LevelOne.h"

int main(int argc, char* argv[])
{
	using namespace thelastforest::scenes;
	using namespace novazero::core;

	// Game Config
	Game game("The Last Forest");
	game.ConfigureDebugOverlay(true);
	n2dSetBackgroundColor("purple");

	n2dAddColor("highlight", "ffffff", 50);

	AllScenes::NewGame();

	MainMenu* mainmenu = new MainMenu("MainMenu");
	LevelOne* level1 = new LevelOne("LevelOne");

	n2dGameAddScene(mainmenu);
	n2dGameAddScene(level1);

	n2dGameConfigFirstScene(mainmenu);

	while (Game::IsRunning())
	{
		game.Tick();
	}

	return Game::s_ExitCode;
}