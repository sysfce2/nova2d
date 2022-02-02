#include "Player.h"
#include "physics/Collision.h"
#include "ai/SimpleFollower.h"
#include "enemies/Pawn.h"
#include "enemies/Leader.h"
#include "specials/PawnBullet.h"

namespace spaceshooter
{
	using namespace novazero::ai;
	using namespace novazero::input;

	Player::Player(std::string assetName, std::string playerNumber, Vec2 position, Vec2Int size, char layer)
		: UDRLController(assetName, position, size, layer), Collider(0)
	{
		m_PlayerNumber = playerNumber;

		n2dAddKeyDownListener(SDLK_SPACE, Player::OnSpace, this);
		n2dAddKeyDownListener(SDLK_ESCAPE, Player::Quit, this);

		if (SDL_NumJoysticks() > 0)
		{
			AddJoyEventListener(0, SDL_CONTROLLER_BUTTON_B, &InputHandler::IsJoystickButtonDown,
				std::bind(&Player::Shoot, this));
		}

		m_Sprite->ConfigureAnimation(0, 2, 100, true);
		ConfigureCollider(m_Sprite, 0, m_PlayerNumber);

		int startX = 8;
		int startY = 8;
		for (int i = 0; i < m_Lives; i++)
		{
			Sprite* life = new Sprite(m_PlayerNumber, Vec2((float)startX + (i * 16), (float)startY), Vec2Int(16, 16), 0);
			m_LifeSprites.push_back(life);
		}

		m_ShootTimer = new Timer(125, true, std::bind(&Player::Shoot, this));

		m_CleanID = n2dAddUpdater(Player::Update, this);

	}

	Player::~Player()
	{
		n2dRemoveUpdater(m_CleanID);
		UDRLController::DestroySelf();
	}

	void Player::Quit()
	{
		n2dSceneChange("mainMenu");
	}

	void Player::OnCollision(Collision* collision)
	{
		bool hit = false;
		if ((collision->m_ColliderA->m_ColliderName == "leader-bullet" && collision->m_ColliderB->m_ColliderName == "player") ||
			(collision->m_ColliderA->m_ColliderName == "pawn-bullet" && collision->m_ColliderB->m_ColliderName == "player"))
		{
			// Bullets with player
			((SimpleFollower*)collision->m_ColliderA)->DestroySelf();
			hit = true;
		}
		else if ((collision->m_ColliderB->m_ColliderName == "leader-bullet" && collision->m_ColliderA->m_ColliderName == "player") ||
			(collision->m_ColliderB->m_ColliderName == "pawn-bullet" && collision->m_ColliderA->m_ColliderName == "player"))
		{
			// Bullets with player
			((SimpleFollower*)collision->m_ColliderB)->DestroySelf();
			hit = true;
		}

		if (hit)
		{
			SmallExplosion();
		}

		if (hit && m_Lives > 1)
		{
			Sprite* s = m_LifeSprites.at(m_Lives - 1);
			s->DestroySelf();
			m_LifeSprites.pop_back();
			m_Lives--;
		}
		else if (hit)
		{
			m_ShootTimer->DestroySelf();

			// Game over
			auto endGame = new auto ([]() {

				n2dSceneChange("gameOver");

			});

			Timer* t = new Timer(1000, false, *endGame);
		}
	}

	void Player::SmallExplosion()
	{
		Sprite* explosion = new Sprite("explode", m_Sprite->GetPosition(), Vec2Int(16, 16), 0);
		explosion->ConfigureAnimation(0, 5, 100, true);
		auto animEnd = new auto ([](Sprite* sprite) {
			sprite->DestroySelf();
			});
		explosion->ConfigureAnimationEnd(*animEnd);
	}

	void Player::Update()
	{
		if (!m_CanSpace)
		{
			if (m_SpaceDelay > 0)
			{
				m_SpaceDelay -= Game::s_DeltaTime;
				return;
			}
			else
			{
				m_CanSpace = true;
				m_SpaceDelay = m_SpaceDelayReset;
			}
		}
	}

	void Player::OnSpace()
	{
		if (!m_CanSpace) return;

		m_CanSpace = false;
		m_CanSpace = m_SpaceDelayReset;

		// shield / or something on space key
	}

	void Player::Shoot()
	{
		// create and shoot bullet
		SimpleBulletController* bullet = new SimpleBulletController(Vec2Int((int)GetX(), (int)GetY() - GetHeight()), Vec2Int((int)GetX(), (int)-GetHeight() - 64), 4);
		bullet->Configure(14, Rect(0, 0, Game::s_Width, Game::s_Height));
		bullet->AddSprite(m_PlayerNumber + "-bullet", Vec2(GetX(), GetY() - GetHeight()), Vec2Int(16, 16), 1);
		bullet->ConfigureCollider(bullet->GetSprite(), 0, m_PlayerNumber + "-bullet");
		bullet->ConfigureAliveBounds(Game::GetGameBounds(32));

		auto collisionFunction = new auto ([](Collision* collision) {

			bool collAisPlayer1Bullet = collision->m_ColliderA->m_ColliderName == "player1-bullet";
			bool collBisPlayer1Bullet = collision->m_ColliderB->m_ColliderName == "player1-bullet";

			bool collAisPlayer2Bullet = collision->m_ColliderA->m_ColliderName == "player2-bullet";
			bool collBisPlayer2Bullet = collision->m_ColliderB->m_ColliderName == "player2-bullet";

			// Bullet with bullet
			if ((collAisPlayer1Bullet || collAisPlayer2Bullet) && collision->m_ColliderB->m_ColliderName == "pawn-bullet")
			{
				((SimpleBulletController*)collision->m_ColliderA)->DestroySelf();
				((PawnBullet*)collision->m_ColliderB)->DestroySelf();
				n2dScoreAdd(2);
			}
			else if(((collBisPlayer1Bullet || collBisPlayer2Bullet) && collision->m_ColliderA->m_ColliderName == "pawn-bullet"))
			{
				((SimpleBulletController*)collision->m_ColliderB)->DestroySelf();
				((PawnBullet*)collision->m_ColliderA)->DestroySelf();
				n2dScoreAdd(2);
			}
			
			// Bullet with pawn
			if ((collAisPlayer1Bullet || collAisPlayer2Bullet) && collision->m_ColliderB->m_ColliderName == "pawn")
			{
				((SimpleBulletController*)collision->m_ColliderA)->DestroySelf();
				((Pawn*)collision->m_ColliderB)->Hurt(4);
			}
			else if ((collBisPlayer1Bullet || collBisPlayer2Bullet) && collision->m_ColliderA->m_ColliderName == "pawn")
			{
				((SimpleBulletController*)collision->m_ColliderB)->DestroySelf();
				((Pawn*)collision->m_ColliderA)->Hurt(4);
			}

			// Bullet with leader
			if ((collAisPlayer1Bullet || collAisPlayer2Bullet) && collision->m_ColliderB->m_ColliderName == "leader")
			{
				((SimpleBulletController*)collision->m_ColliderA)->DestroySelf();
				((Leader*)collision->m_ColliderB)->Hurt(2);
			}
			else if ((collBisPlayer1Bullet || collBisPlayer2Bullet) && collision->m_ColliderA->m_ColliderName == "leader")
			{
				((SimpleBulletController*)collision->m_ColliderB)->DestroySelf();
				((Leader*)collision->m_ColliderA)->Hurt(2);
			}

		});

		bullet->ConfigureOnCollision(*collisionFunction);

	}
}