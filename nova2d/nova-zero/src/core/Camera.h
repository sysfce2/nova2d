#pragma once
#include "Positional.h"
#include "BoundUser.h"
#include "../maths/Rect.h"
#include "../utils/Tweens.h"

namespace novazero
{
	namespace core
	{
		using namespace utils;

		class Camera : 
			public BoundUser
		{

		private:

			unsigned int m_CleanID = 0;
			unsigned int m_ID = 0;

			Rect m_DrawArea = Rect(0, 0, 0, 0);
			Vec2 m_Offset = Vec2(0, 0);
			float m_Zoom = 1.f;
			float m_OldZoom = 1.f;
			float m_ZoomSpeed = 0.01f;
			float m_CameraPadding = 64.f;

			Positional* m_FollowTarget = nullptr;

			TweenTypes m_FollowType = TweenTypes::EaseInCubic;
			float m_FollowSpeed = 1000.f;

			bool m_FreeMove = false;
			void FreeMove();

		public:

			Camera(Rect bounds = Rect(0, 0, 0, 0));

			void SetFollowTarget(Positional* target, float followSpeed, bool startOnTargetPosition, float zoomLevel, float distanceBeforeFollow,
				TweenTypes followType);

			void SetCameraPadding(float padding)
			{
				m_CameraPadding = padding;
			}

			void SetFollowType(TweenTypes type)
			{
				m_FollowType = type;
			}

			void SetFollowSpeed(float speed)
			{
				m_FollowSpeed = speed;
			}

			void Reset();

			void Update();
			void FollowTarget();

			Vec2 GetWorldPosition() const;

			void CenterOn(Positional* target, float zoom = -1.f);
			void CenterOn(Vec2 position, float zoom = -1.f);
			Vec2 ReverseCenterOn(Vec2 position);

			Vec2 GetPosition() const;
			Vec2Int GetPositionInt() const;
			float GetX() const;
			float GetY() const;

			void EnableFreeWASDMove(bool enabled);
			bool IsFreeMoveEnabled() const;

			float GetZoom() const;
			void SetZoom(float zoomLevel, bool force = false);

			Vec2 GetDrawRectCenter() const;
			void SetDrawRect(Rect drawRect);
			void SetDrawRectPosition(Vec2 pos);
			Rect GetDrawRect() const;
			Vec2 GetDrawRectPosition() const;
			Vec2 GetDrawRectSize() const;

			void DestroySelf() override;

		};
	}
}