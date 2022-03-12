#pragma once
#include "../graphics/Sprite.h"
#include "../core/EventListener.h"
#include "../maths/Vec2Int.h"
#include "../utils/TextureLoader.h"
#include "../core/Game.h"
#include "../physics/TimeEffected.h"
#include "../core/Positional.h"

namespace novazero
{
	namespace controllers
	{
		using namespace core;
		using namespace graphics;
		using namespace maths;

		class SimpleController :
			public EventListener,
			public TimeEffected,
			public Positional
		{

		private:

			Sprite* m_Sprite = nullptr;

		protected:

		public:

			SimpleController(const std::string& assetName, Vec2 position, Vec2Int size, unsigned char layer);

			void SetMoveSpeed(float mSpeed);
			
			inline float GetMoveSpeed() const { return m_MoveSpeed; }

			void DestroySelf();

		private:

			bool m_UsingAcceleration = false;

		protected:

			void SetAcceleration(TweenTypes type, float accelerationTimeMS, float deaccelerationTimeMS);
			bool IsUsingAcceleration() { return m_UsingAcceleration; }

		public:

			unsigned int m_AccelerationTweenX = 0;
			unsigned int m_AccelerationTweenY = 0;
			float m_MoveSpeed = 0.0f;
			
			bool m_AcceleratingX = false;
			bool m_DeacceleratingX = false;
			bool m_AcceleratingY = false;
			bool m_DeacceleratingY = false;

			float m_CurrentAccelerationX = 0.0f;
			float m_CurrentAccelerationY = 0.0f;

			TweenTypes m_AccelerationType = TweenTypes::EaseInCubic;
			float m_TotalAccelerationSpeedMS = 1000.f;
			float m_TotalDeaccelerationSpeedMS = 1000.f;

			void EnableAcceleration(bool v) { m_UsingAcceleration = v; }

		};
	}
}