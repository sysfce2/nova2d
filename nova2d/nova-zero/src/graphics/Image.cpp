#include "Image.h"
#include "../core/Game.h"

namespace novazero
{
	namespace graphics
	{
		using namespace core;

		Image::Image(const std::string& assetName, Vec2 position, Vec2Int size, unsigned char layer)
			: Deleteable("image_"), Drawable(size)
		{
			m_ID = n2dGameGetID();
			m_DeleteName = "image_" + tostring(m_ID);
			m_Layer = layer;

			m_AssetName = assetName;

			SetPosition(position);

			m_Angle = 0;
			m_Layer = layer;

			m_ImageTexture = Game::s_AssetManager->GetTexture(assetName);
			m_FrameSize = size;

			// Draw setup
			m_SrcRect.x = 0;
			m_SrcRect.y = 0;
			m_SrcRect.w = (int)size.x;
			m_SrcRect.h = (int)size.y;
			m_DestRect.w = (int)size.x;
			m_DestRect.h = (int)size.y;

			LinkPositionalDrawable(this);

			n2dAddDrawable(this, layer);
		}

		void Image::SwapTexture(const std::string& assetName)
		{
			if (m_ImageTexture)
			{
				if (m_AssetName == assetName) return;
				m_ImageTexture = n2dAssetsGetTexture(assetName);
			}
			else
			{
				m_ImageTexture = n2dAssetsGetTexture(assetName);
			}

			m_AssetName = assetName;
		}

		void Image::SetScale(float scale)
		{
			if (GetDrawable())
				GetDrawable()->SetDrawScale(scale);

			m_DestRect.w = (int)(m_FrameSize.x * scale);
			m_DestRect.h = (int)(m_FrameSize.y * scale);
		}

		void Image::Draw(float oX, float oY, float scale)
		{
			if (!IsVisible()) return;

			m_SrcRect.x = 0;
			m_SrcRect.w = m_FrameSize.x;

			if (IsFixed())
			{
				m_DestRect.x = (int)(GetX() + OffsetX());
				m_DestRect.y = (int)(GetY() + OffsetY());
			}
			else
			{
				m_DestRect.x = (int)(oX + OffsetX());
				m_DestRect.y = (int)(oY + OffsetY());
			}

			int w = m_DestRect.w;
			int h = m_DestRect.h;

			m_DestRect.w = (int)(w * scale);
			m_DestRect.h = (int)(h * scale); 
			
			SDL_RenderCopyEx(Game::s_Renderer->GetSDLRenderer(), m_ImageTexture, &m_SrcRect, &m_DestRect, m_Angle, NULL, m_Flip);

			m_DestRect.w = w;
			m_DestRect.h = h;
		}

		bool Image::operator==(const Image& other)
		{
			return m_ID == other.m_ID;
		}

		void Image::DestroySelf()
		{
			n2dRemoveDrawable(m_ID, m_Layer);

			if (m_ImageTexture)
				SDL_DestroyTexture(m_ImageTexture);

			SetDeleted(true);
		}
	}
}