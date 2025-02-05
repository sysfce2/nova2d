#pragma once
#include "Color.h"
#include "../graphics/DrawLayers.h"
#include "../utils/ColorManager.h"

namespace novazero
{
	namespace core
	{
		class Game;
	}

	namespace graphics
	{
		using namespace utils;

		class Renderer
		{

		private:

			SDL_Renderer* m_Renderer;
			Color* m_BackgroundColor;
			
		public:

			static Color* s_DebugRenderColor;
			static Color* s_DebugRenderColorSleep;
			static Color* s_DebugRenderColorStatic;

		public:

			Renderer(SDL_Window& window, Color* backgroundColor);

			inline SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

			void SetBlendMode(bool enableAlpha);
			
			void SetBackgroundColor(const char* colorName);
			void SetDrawColor(const char* colorName, Uint8 alphaOverride = 255);

			void PreDraw() const;
			void Draw() const;
			void DebugDraw();
			void PostDraw() const;

			void DestroySelf();

		public:

			static DrawLayers* s_DrawLayers;

		};
	}
}