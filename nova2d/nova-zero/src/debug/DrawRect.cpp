#pragma once
#include "DrawRect.h"
#include "../graphics/Drawable.h"
#include "../core/Game.h"

namespace novazero
{
	namespace debug
	{
		using namespace graphics;

		DrawRect::DrawRect(const std::string& fillColor, const std::string& outlineColor,
			bool filled, Rect rect, int outlineThickness, unsigned char layer)
			: Drawable(Vec2Int((int)rect.w, (int)rect.h)), Deleteable("rect_")
		{
			m_DeleteName = "rect_" + std::to_string(m_ID);

			m_Layer = layer;

			m_FillColor = fillColor;
			m_OutlineColor = outlineColor;
			m_Filled = filled;

			SetPositionInt(Vec2Int((int)rect.x, (int)rect.y));

			m_Outline = new SDL_Rect();
			m_Fill = new SDL_Rect();

			m_Outline->x = (int)rect.x;
			m_Outline->y = (int)rect.y;
			m_Outline->w = (int)rect.w;
			m_Outline->h = (int)rect.h;

			m_Thickness = outlineThickness;

			m_Fill->x = (int)rect.x + outlineThickness;
			m_Fill->y = (int)rect.y + outlineThickness;
			m_Fill->w = (int)rect.w - 2 * outlineThickness;
			m_Fill->h = (int)rect.h - 2 * outlineThickness;

			n2dAddDrawable(this, layer);

		}

		void DrawRect::SetColors(const std::string& fillColor, const std::string& outlineColor)
		{
			m_FillColor = fillColor;
			m_OutlineColor = outlineColor;
		}

		void DrawRect::DestroySelf()
		{
			n2dRemoveDrawable(m_ID, m_Layer);
			CleanUpdaters();

			SetDeleted(true);
		}

		void DrawRect::Draw(float oX, float oY, float scale)
		{
			if (!m_Visible) return;

			int fillW = m_Fill->w;
			int fillH = m_Fill->h;

			m_Fill->w = (int)(fillW * scale);
			m_Fill->h = (int)(fillH * scale);
			
			if (IsFixed())
			{
				m_Fill->x = (int)(GetX() + m_Thickness);
				m_Fill->y = (int)(GetY() + m_Thickness);

				m_Outline->x = (int)(GetX());
				m_Outline->y = (int)(GetY());
			}
			else
			{
				m_Fill->x = (int)(oX + m_Thickness);
				m_Fill->y = (int)(oY + m_Thickness);

				m_Outline->x = (int)(oX);
				m_Outline->y = (int)(oY);
			}

			int outlineW = m_Outline->w;
			int outlineH = m_Outline->h;

			m_Outline->w = (int)(outlineW * scale);
			m_Outline->h = (int)(outlineH * scale);

			Uint8 r, g, b, a;
			SDL_GetRenderDrawColor(Game::s_Renderer->GetSDLRenderer(), &r, &g, &b, &a);

			if (m_Filled)
			{
				if (!m_Fill)
				{
					LOG(LVL_WARNING, "Cannot draw rect", __FILE__, __LINE__);
					return;
				}

				n2dRenderDrawColor(m_FillColor);
				SDL_RenderFillRect(Game::s_Renderer->GetSDLRenderer(), m_Fill);
				n2dRenderDrawColor(m_OutlineColor);
				SDL_RenderDrawRect(Game::s_Renderer->GetSDLRenderer(), m_Outline);
			}
			else
			{
				if (!m_Outline)
				{
					LOG(LVL_WARNING, "Cannot draw rect", __FILE__, __LINE__);
					return;
				}

				n2dRenderDrawColor(m_OutlineColor);
				SDL_RenderDrawRect(Game::s_Renderer->GetSDLRenderer(), m_Outline);
			}

			SDL_SetRenderDrawColor(Game::s_Renderer->GetSDLRenderer(), r, g, b, a);

			m_Fill->w = fillW;
			m_Fill->h = fillH;

			m_Outline->w = outlineW;
			m_Outline->h = outlineH;
		}
	}
}