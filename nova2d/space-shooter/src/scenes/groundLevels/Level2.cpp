#pragma once
#include "Level2.h"

namespace spaceshooter
{
	using namespace novazero::core;
	using namespace novazero::maps;
	using namespace novazero::graphics;

	Level2::Level2(const std::string& sceneName)
		: Scene(sceneName)
	{

	};

	void Level2::Update()
	{
		
	}

	void Level2::Start()
	{
		Game::SetGamePadding(0, 0, 0, 0);

		// Maps
		n2dAssetsLoadAndAddMap("level2", "res/maps/level2/level2.json",
			"res/maps/level2/level2.png", "res/maps/level2/level2-tileset.json");

		GroundPlayer* player = new GroundPlayer(Vec2(200, Game::s_Height / 2), Vec2Int(16, 16), 0);
		CAMERA->SetFollowTarget((Positional*)player, 2.f, true, 2.f, 400.f, TweenTypes::EaseInExpo);

	}

	void Level2::OnEscape()
	{

	}

	void Level2::End()
	{

	}
}