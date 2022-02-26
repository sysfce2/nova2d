#include "Pawn.h"

namespace spaceshooter
{
	Pawn::Pawn(const std::string& assetName, Vec2 position, Vec2Int size, char layer, float* moveUpdateDelay,
		float shootMin, float shootMax)
		: SimpleWeakAI()
	{
		m_DeleteName = assetName + tostring(SimpleWeakAI::Collider::m_ID);

		SetPosition(position);
		AddSprite(assetName, position, size, layer);
		ConfigureCollider(GetSprite(), 0, "pawn");
		ConfigureUsingBounds(false, false);

		m_HealthBar = new SimpleStatBar(false, (int)GetX(), (int)GetY() - 2,
			16, 4, "light-blue", "bright-blue", "white", layer);
		m_HealthBar->ConfigureThickness(1);
		m_HealthBar->ConfigureForeground("white", "yellow", "red");

		auto cleanID = n2dAddUpdater(Pawn::PawnUpdate, this);
		m_CleanUpdaters.push_back(cleanID);

		Configure(moveUpdateDelay, false);
		AddPatrolPointWithFunction(Vec2(GetX(), GetY() + 200), GetLinearPatrolMove());
		
		EnableAI(true);

		n2dAddDeleteable(this);
	}

	void Pawn::PawnUpdate()
	{
		if (!m_Alive) return;

		m_HealthBar->Update(m_Health * 2, (int)GetX(), (int)GetY() - 8);

	}

	void Pawn::DisplayHit(int damage)
	{
		Vec2 pos = GetPosition();
		int width = GetWidth();

		HitDisplay* hitDisplay = new HitDisplay("+ " + std::to_string(damage), "font4", "green", Rect(pos.x - width / 2, GetY() - 16.f, 24.f, 16.f),
			Vec2(pos.x, pos.y - 128), 4000, 0);
	}

	void Pawn::Hurt(int damage)
	{
		SmallExplosion();
	}

	void Pawn::SmallExplosion()
	{
		Sprite* explosion = new Sprite("explode", GetPosition(), Vec2Int(16, 16), 0);
		explosion->ConfigureAnimation(0, 5, 5, 100, true);
		auto animEnd = new auto ([](Sprite* sprite) {
			sprite->DestroySelf();
		});
		explosion->ConfigureAnimationEnd(*animEnd);
	}

	void Pawn::DestroySelf()
	{
		if (m_Destroyed) return;
		m_Destroyed = true;

		m_Alive = false;

		m_UsingCollider = false;

		if (m_HealthBar)
			m_HealthBar->DestroySelf();

		if (GetSprite())
			GetSprite()->DestroySelf();

		m_Dead = 1;
	}
}