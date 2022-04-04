#pragma once
#include "core/Scene.h"
#include "maps/TileMap.h"
#include "../../actors/Player.h"
#include "../AllScenes.h"
#include "../../controllers/HumanController.h"
#include "../../controllers/TreeController.h"
#include "../../controllers/ResourceController.h"

namespace thelastforest
{
	namespace scenes
	{
		using namespace novazero::core;
		using namespace novazero::maps;
		using namespace actors;
		using namespace controllers;

		class LevelOne :
			public Scene
		{

		private:

			TileMap* m_Background = nullptr;
			ResourceController* m_ResourceController = nullptr;
			Player* m_Player = nullptr;

		public:

			HumanController* m_HumanController = nullptr;
			TreeController* m_TreeController = nullptr;

		public:

			LevelOne(const std::string& sceneName);

			void CreateWorld();
			void SetupLevel();
			void CheckGameOver();

			void SetupHumans();
			void SetupTrees();
			void SetupResources();

			void Start() override;
			void Update() override;
			void End() override;

		};
	}
}