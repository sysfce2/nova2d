#include "Leader.h"
#include "ai/SimpleFollower.h"
#include "physics/Collision.h"

namespace spaceshooter
{
	Leader::Leader(std::string assetName, Vec2Int position, Vec2Int size, char layer)
	{
		AddSprite(assetName, position, size, layer);
		
		int forwardMove = 96;
		AddPatrolPointWithFunction(Vec2Int(position.x, position.y + forwardMove), std::bind(&Leader::LinearPatrolMove, this));

		AddPatrolPointWithFunction(Vec2Int(position.x, position.y + forwardMove), std::bind(&Leader::LinearPatrolMove, this));
		AddPatrolPointWithFunction(Vec2Int(position.x - 300, position.y + forwardMove), std::bind(&Leader::LinearPatrolMove, this));
		AddPatrolPointWithFunction(Vec2Int(position.x, position.y + forwardMove), std::bind(&Leader::LinearPatrolMove, this));
		AddPatrolPointWithFunction(Vec2Int(position.x + 300, position.y + forwardMove), std::bind(&Leader::LinearPatrolMove, this));
		
		EnableAI(true);
		Configure(1, true);
		ConfigureLoopIndex(1);
		ConfigureShoot(2000, 5000);

		for (int row = 0; row <= 2; row++)
		{
			for (int col = -3; col <= 3; col++)
			{
				int	offsetX = col * 96;
				int offsetY = 64 + (row * 32);

				Pawn* pawn = new Pawn("pawn", Vec2Int(position.x + offsetX, offsetY), Vec2Int(16, 16), 1, m_Sprite, 0.0f);
				pawn->Configure(m_MoveSpeed + 2);
				pawn->ConfigureShoot(5000, 10000);
				pawn->Offset(Vec2Int(offsetX, offsetY));

				m_Pawns.push_back(pawn);

			}
			
		}

		ConfigureCollider(m_Sprite, 0, "leader");

		m_HealthBar = new SimpleStatBar(false, m_Sprite->GetX(), m_Sprite->GetY() - 2,
			32 * 2, 12, "light-blue", "bright-blue", "white", layer);
		m_HealthBar->ConfigureThickness(1);
		m_HealthBar->ConfigureForeground("white", "yellow", "red");

		n2dAddUpdater(Leader::Update, this);
		n2dAddUpdater(Leader::ShootUpdate, this);
		n2dAddUpdater(Leader::HealthUpdate, this);
	}

	void Leader::HealthUpdate()
	{
		m_HealthBar->Update(m_Health * 2, m_Sprite->GetX() - 24, m_Sprite->GetY() - 16);
	}

	void Leader::Hurt(int damage)
	{
		SmallExplosion();

		m_Health -= damage;
		if (m_Health < 1)
		{
			DestroySelf();
		}
	}

	void Leader::SmallExplosion()
	{
		Sprite* explosion = new Sprite("explode", m_Sprite->GetPosition(), Vec2Int(16, 16), 0);
		explosion->ConfigureAnimation(0, 5, 100, true);
		auto animEnd = new auto ([](Sprite* sprite) {
			sprite->DestroySelf();
			});
		explosion->ConfigureAnimationEnd(*animEnd);
	}

	Leader::~Leader()
	{
		n2dRemoveUpdater(Leader::Update, this);
		n2dRemoveUpdater(Leader::ShootUpdate, this);
		n2dRemoveUpdater(Leader::HealthUpdate, this);
	}

	void Leader::ShootUpdate()
	{
		if (m_DelayShoot < 0)
		{
			m_DelayShoot = randomf(1, m_DelayShootMax);
			Shoot();
			return;
		}

		m_DelayShoot = (float)(m_DelayShoot - Game::s_DeltaTime);
	}

	void Leader::Shoot()
	{
		
		Player* player = (Player*)SceneManager::s_ReferenceManager->GetReferenced("player");
		SimpleFollower* bullet = new SimpleFollower(player->GetSprite(), 0.0f);
		bullet->AddSprite("leader-bullet", Vec2Int(GetX(), GetY() + 32), Vec2Int(16, 16), 1);
		bullet->ConfigureCollider(bullet->GetSprite(), 0, "leader-bullet");
		// Todo: fix this
		//bullet->ConfigureAliveBounds(Rect(-16, -16, Game::s_Width + 16, Game::s_Height + 16));
		bullet->Configure(1);
		bullet->ConfigureRotation(true, -90);
		
		auto onCollision = new auto ([](Collision* collision)
		{
			if ((collision->m_ColliderA->m_ColliderName == "leader-bullet" &&
				collision->m_ColliderB->m_ColliderName == "player-bullet"))
			{
				((SimpleFollower*)collision->m_ColliderA)->DestroySelf();
				((SimpleBulletController*)collision->m_ColliderB)->DestroySelf();
			}
			else if ((collision->m_ColliderB->m_ColliderName == "leader-bullet" &&
				collision->m_ColliderA->m_ColliderName == "player-bullet"))
			{
				((SimpleFollower*)collision->m_ColliderB)->DestroySelf();
				((SimpleBulletController*)collision->m_ColliderA)->DestroySelf();
			}
		});

		bullet->ConfigureOnCollision(*onCollision);

		Timer* bulletDestruct = new Timer(randomf(6500, 12000), false, std::bind(&SimpleFollower::DestroySelf, bullet));
		bullet->AddTimer(bulletDestruct);

	}

	void Leader::DestroySelf()
	{
		if (m_Destroyed) return;

		m_Destroyed = true;

		n2dRemoveUpdater(Leader::Update, this);

		if (m_Sprite)
			delete m_Sprite;

		if (m_HealthBar)
			delete m_HealthBar;
	}
}