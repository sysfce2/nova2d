#pragma once
#include "../core/Defines.h"
#include "../core/TypeDefs.h"
#include "SDL.h"

#include "../input/InputHandler.h"
#include "../graphics/Window.h"

#include "SceneManager.h"
#include "../utils/ColorManager.h"
#include "../utils/AssetManager.h"
#include "../physics/CollisionManager.h"
#include "../utils/FontManager.h"
#include "../utils/SQLManager.h"
#include "../debug/DebugOverlay.h"
#include "../utils/Director.h"

#include "../graphics/Color.h"
#include <vector>
#include "../maths/Rect.h"

namespace novazero
{

	namespace graphics
	{
		class Renderer;
	}

	namespace core
	{
		class Game
		{

		private:

			const char FPS = 60;
			const float FRAME_DELAY = 1000.0f / FPS;
			Uint32 frameStart = 0;
			int frameTime = 0;
			Uint64 NOW;
			Uint64 LAST;

			const char* m_Title = {};
			
			novazero::graphics::Window* m_MainWindow = nullptr;
			novazero::core::SceneManager* m_SceneManager = nullptr;

		public:

			Game(const char* title, const novazero::maths::Vec2Int screenSize = novazero::maths::Vec2Int(1280, 800));
			~Game();

			void Tick();

			void Process();
			void Update();
			void PollEvents();
			void Render();
			void Clean();

			void ConfigureFirstScene(const std::string& sceneName);
			void ConfigureIcon(const char* iconPath) { m_MainWindow->ConfigureIcon(iconPath); }
			void ConfigureSQL(const std::string& databaseName, const std::string& connectionString, const std::string& user,
				const std::string& pass, bool useNovaSQLScoring);

		public:

			static novazero::graphics::Renderer* s_Renderer;
			static novazero::utils::ColorManager* s_ColorManager;
			static novazero::input::InputHandler* s_InputHandler;
			static novazero::utils::AssetManager* s_AssetManager;
			static SceneManager* s_SceneManager;
			static novazero::utils::FontManager* s_FontManager;
			static novazero::utils::SQLManager* s_SQLManager;
			static novazero::debug::DebugOverlay* s_DebugOverlay;
			static novazero::utils::Director* s_Director;

			static SDL_KeyCode s_PauseKey;
			static void PauseGame(bool pause);

			static void ConfigureDebug(bool isVisible);
			// Must call ConfigureDebug(true) before this works
			static void ConfigureDebugPosition(Vec2Int pos) { if (s_DebugOverlay) { s_DebugOverlay->SetPositionInt(pos); } }

			static double s_DeltaTime;
			static double GetDeltaTime() { return s_DeltaTime; }
			static double s_FPS;

			static int s_Width;
			static int s_Height;
			static int s_Padding;

			static Vec2 GetCenterScreen(int xCorrection = 0, int yCorrection = 0) 
			{
				return Vec2((float)s_Width / 2 - xCorrection, (float)s_Height / 2 - yCorrection);
			}
			static novazero::maths::Rect GetGameBounds(float paddingOverride = 0.f);
			static void SetGamePadding(float padding);
			
			static unsigned int s_IDCount;
			static unsigned int GetNextID() { s_IDCount++; return s_IDCount; }

			static unsigned int s_Score;
			static unsigned int AddScore(unsigned int add) { s_Score += add; return s_Score; }
			static unsigned int GetScore() { return s_Score; }
			static void SetScore(unsigned int score) { s_Score = score; }

			static bool s_Debug;
			static bool IsDebug() { return s_Debug; }
			static void SetDebug(bool val) { s_Debug = val; }

			static float s_TimeScale;
			static float s_TimeScaleMemory;

			static bool s_Running;
			static bool IsRunning() { return s_Running; }
			static int s_ExitCode;
			static void EndGame(int exitCode)
			{
				s_ExitCode = exitCode;
				s_Running = false;
			}

			static bool CoinFlip()
			{
				auto r = n2dRandomFloat(1, 10);
				return r > 5;
			}

			static float RandomFloat(float min, float max)
			{
				return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
			}
			static float RandomFloatWithMaxChance(float min, float max, float chanceOfMax)
			{
				float chance = n2dRandomFloat(0, 1);
				if (chance < chanceOfMax) return max;
				return n2dRandomFloat(min, max);
			}
			static float RandomFloatWithMinChance(float min, float max, float chanceOfMin)
			{
				float chance = n2dRandomFloat(0, 1);
				if (chance < chanceOfMin) return min;
				return n2dRandomFloat(min, max);
			}
			static int RandomInt(int min, int max)
			{
				// Random int between min-max
				return (int)(rand() % min + max);
			}
			static int RandomIntWithMaxChance(int min, int max, float chanceOfMax)
			{
				float chance = n2dRandomFloat(0, 1);
				if (chance < chanceOfMax) return max;
				return n2dRandomInt(min, max);
			}
			static int RandomIntWithMinChance(int min, int max, float chanceOfMin)
			{
				float chance = n2dRandomFloat(0, 1);
				if (chance < chanceOfMin) return min;
				return n2dRandomInt(min, max);
			}
			
		};
	}
}