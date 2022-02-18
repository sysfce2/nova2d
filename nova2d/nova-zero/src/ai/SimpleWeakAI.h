#pragma once
#include <functional>
#include <vector>
#include "../maths/Vec2Int.h"
#include "../graphics/Sprite.h"
#include "../core/Game.h"
#include "../core/BoundUser.h"
#include "../physics/Collider.h"
#include "../core/Deleteable.h"
#include "../physics/TimeEffected.h"
#include "../core/Positional.h"

typedef std::function<void()> f_MovePtrFunction;

namespace novazero
{
	namespace ai
	{
		using namespace maths;
		using namespace graphics;
		using namespace core;
		using namespace physics;

		struct PatrolVec2IntWithMoveFunction
		{
			Vec2 patrolVec2;
			f_MovePtrFunction moveFunction;
		};

		class SimpleWeakAI : 
			public BoundUser, 
			public Collider, 
			public Deleteable,
			public TimeEffected,
			public Positional
		{

		private:

			std::vector<f_MovePtrFunction> m_MoveFunctions;

			bool m_LoopMoving = true;
			int m_PatrolIndex = -1;

			int m_LoopStartIndex = 0;
			std::function<void()> f_OnPatrolComplete = nullptr;
			float m_DelayMS = 1000;
			int m_DelayMaxMS = 1000;

			Vec2 m_MemoryMovement;
			bool m_ContinueAfterPatrolComplete = false;
			
			std::vector<Vec2*> m_PatrolPoints;

		public:

			SimpleWeakAI();

			void Configure(int patrolDelayMS, bool loop) 
			{ 
				m_DelayMaxMS = patrolDelayMS;
				m_LoopMoving = true;
			}

			void LookAt(Vec2Int target, int extraRotation)
			{
				int lookAtAngle = Vec2Int::LookAtAngle(Vec2Int((int)GetX(), (int)GetY()), 
					target, extraRotation);
				GetSprite()->SetAngle(lookAtAngle);
			}

			void ConfigureContinueAfterPatrolComplete(bool continueAfter) { m_ContinueAfterPatrolComplete = continueAfter; }
			void ConfigureOnPatrolComplete(std::function<void()> f) { f_OnPatrolComplete = f; }
			void ConfigureLoopIndex(int idx) { m_LoopStartIndex = idx; }
			void RestartPatrol() { m_PatrolIndex = 0; }
			void EnableAI(bool isEnabled);
			void ResetAI();

			void SetAllPatrolPoints(std::vector<Vec2*> points);
			std::vector<Vec2*> GetPatrolPoints()
			{
				return m_PatrolPoints;
			}
			Sprite* GetSprite()
			{
				return Positional::GetLinkedSprite();
			}

			inline void SetVisible(bool isVisible) 
			{ 
				if (GetSprite())
					GetSprite()->m_Visible = isVisible;
			}

			void AddSprite(const std::string& assetName, Vec2 position, Vec2Int size, char layer);

			void AddPatrolPointWithFunction(Vec2* point, f_MovePtrFunction func);
			void AddPatrolPointWithFunction(Vec2 point, f_MovePtrFunction func);
			void RemovePatrolPointWithFunction(Vec2* point);

			void SetAllPatrol(std::vector<Vec2*> points, std::vector<f_MovePtrFunction> funcs);
			void ClearPatrol();

			void LinearPatrolMove();
			std::function<void()> GetLinearPatrolMove() { return std::bind(&SimpleWeakAI::LinearPatrolMove, this); }
			
			void Update();
			void OnCollision(Collision* collision) override { };

			void DestroySelf();

		};
	}
}