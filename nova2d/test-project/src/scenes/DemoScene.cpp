#include "DemoScene.h"
#include "logging/logging.h"
#include "core/Game.h"

namespace testproject
{
	DemoScene::DemoScene(const std::string& sceneName)
		: Scene(sceneName)
	{
		
	}

	void DemoScene::Start()
	{
		m_ContactListener = new DemoSceneContactListener();
		EnablePhysics(true, m_ContactListener);
		PhysicsEnableDebug(true);

		n2dAssetsLoadAndAddTexture("player", "res/lander-TX518.png");
		n2dAssetsLoadAndAddTexture("alien", "res/Ralph-TX18.png");
		n2dAssetsLoadAndAddTexture("planet", "res/planet.png");
		n2dAssetsLoadAndAddTexture("bullet", "res/bullet.png");
		n2dAssetsLoadAndAddTexture("fuel", "res/fuel.png");
		n2dAssetsLoadAndAddTexture("jets", "res/jets.png");

		planet = new Image("planet", Vec2(Game::s_Width - 256, Game::s_Height - 256),
			Vec2Int(256, 256), 2);

		player = new Player(Game::GetCenterScreen(), Vec2Int(32, 32),
			10);
		player->ConfigurePhysicsRect("player", false);
		n2dReferenceAdd("player", player);

		alien = new Alien("alien", Vec2(50, 50), Vec2Int(32, 32), 2);

	}

	void DemoScene::Update()
	{
		
	}

	void DemoScene::End()
	{

	}

	void DemoScene::DestroySelf()
	{

	}
}