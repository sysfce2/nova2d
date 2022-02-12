#pragma once
#include "ai/SimpleWeakAI.h"
#include "Pawn.h"
#include <vector>
#include "../Player.h"
#include "components/SimpleStatBar.h"
#include "core/Referenceable.h"

namespace spaceshooter
{
	using namespace novazero::ai;
	using namespace novazero::controllers;
	using namespace novazero::core;
	using namespace novazero::components;

	class Leader
		: public SimpleWeakAI, public Referenceable
	{
		
	private:

		float m_DelayShoot = 0.0f;
		float m_DelayShootMin = 1.0f;
		float m_DelayShootMax = 5.0f;

		SimpleStatBar* m_HealthBar = nullptr;
		int m_HealthMax = 64;
		int m_Health = 64;
		std::string m_KilledBy;

		std::vector<Vec2*> m_PatrolMemory;
		bool m_Retreating = false;
		bool m_GeneratingPawnWave = false;

		Timer* m_BombTimer = nullptr;
		Timer* m_MoveTimer = nullptr;
		Timer* m_GenTimer = nullptr;

	public:
	
		static std::vector<Pawn*> s_Pawns;
		static int s_PawnCount;
	
	public:

		Leader(const std::string& assetName, Vec2 position, Vec2Int size, int maxHealth, int pawnRows, int pawnCols, char layer);

		void ConfigureShoot(float minShootDelayMS, float maxShootDelayMS) { m_DelayShootMin = minShootDelayMS * 2; m_DelayShootMax = maxShootDelayMS * 2; }

		void ShootUpdate();
		void Shoot();

		void WatchPawns();
		void GeneratePawnWave(char rows, char cols);
		void Retreat();

		void MoveForwardThenBack();
		void RememberOldMoving();
		void PawnAttack();
		void ClearDeadPawns();
		void RemovePawn(unsigned int pawnID);

		void DeployBomb();

		void HealthUpdate();
		void DisplayHit(int score, Vec2 pos);
		void Hurt(int damage, const std::string& damager = "player0");
		void SmallExplosion();

		void DestroySelf() override;

	public:

		static int s_PawnWave;

	};
}