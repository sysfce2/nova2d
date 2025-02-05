#pragma once
#include "../core/Positional.h"
#include "SDL.h"

namespace novazero
{
	namespace graphics
	{
		using namespace core;

		class DrawableCollection;

		class Drawable : public Positional
		{

		private:

			DrawableCollection* m_DrawableCollection = nullptr;
			bool m_Visible = true;
			Vec2Int m_Size;
			float* m_Scale = nullptr;
			bool m_Fixed = false; //     WILL NOT MOVE WITH CAMERA if fixed
			bool m_NonScale = false; //  WILL NOT SCALE WITH CAMERA if non-scale

		protected:
			
			Drawable(Vec2Int size);

		public:

			unsigned char m_Layer = 0;
			SDL_RendererFlip m_Flip = SDL_FLIP_NONE;

			void Flip(SDL_RendererFlip flip)
			{
				m_Flip = flip;
			}

			void SetDrawableCollection(DrawableCollection* collection);
			void ClearDrawableCollection(unsigned int id);

			virtual void Draw(float oX = 0.f, float oY = 0.f, float scale = 1.f) = 0;

			void SetFixedAndNonScale(bool val)
			{
				m_Fixed = val;
				m_NonScale = val;
			}

			void SetFixed(bool f) 
			{ 
				m_Fixed = f; 
			}

			bool IsFixed()const
			{ 
				return 
					m_Fixed; 
			}

			void SetNonScale(bool doesNotScale) 
			{ 
				m_NonScale = doesNotScale; 
			}

			bool IsNotScaleable()const 
			{ 
				return m_NonScale; 
			}

			void SetDrawScale(float scale) 
			{ 
				*m_Scale = scale; 
			}
			float GetDrawScale() const 
			{ 
				return *m_Scale; 
			}

			float* GetDrawScaleRef() const
			{
				return m_Scale;
			}

			int GetWidth() const 
			{
				return (int)(m_Size.x * (*m_Scale));
			}

			int GetHeight() const
			{
				return (int)(m_Size.y * (*m_Scale));
			}

			void SetWidth(int w) 
			{ 
				m_Size.x = w; 
			}

			void SetHeight(int h) 
			{ 
				m_Size.y = h; 
			}

			void SetSize(Vec2Int size) { m_Size = size; }
			Vec2Int GetSize() const { return m_Size; }
			Vec2 GetCenter() const 
			{
				return Vec2(-(GetWidth() * (*m_Scale)) / 2, -(GetHeight() * (*m_Scale)) / 2);
			}

			float GetAngle() const { return m_Angle; }

			void SetOrigin(float x, float y);
			void OriginCenter();
			void OriginTopLeft();

			void SetVisible(bool isVisible) 
			{ 
				m_Visible = isVisible; 
			}

			bool IsVisible() const 
			{ 
				return m_Visible; 
			}

			virtual void DestroySelf() = 0;

		};
	}
}