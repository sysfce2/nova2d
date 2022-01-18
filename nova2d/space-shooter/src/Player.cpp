#include "Player.h"
#include "physics/Collision.h"
#include "ai/SimpleFollower.h"

namespace spaceshooter
{
	using namespace novazero::ai;

	Player::Player(std::string assetName, Vec2Int position, Vec2Int size, char layer)
		: UDRLController(assetName, position, size, layer), Collider(0)
	{
		AddKeysEventListener(SDLK_SPACE, &InputHandler::IsKeyDown, std::bind(&Player::Shoot, this));
		
		if (SDL_NumJoysticks() > 0)
		{
			AddJoyEventListener(0, SDL_CONTROLLER_BUTTON_B, &InputHandler::IsJoystickButtonDown,
				std::bind(&Player::Shoot, this));
		}

		m_Sprite->ConfigureAnimation(0, 2, 100, true);
		ConfigureCollider(m_Sprite, 0, "player");

		Game::AddUpdater(std::bind(&Player::Update, this));
	}

	Player::~Player()
	{
		Game::RemoveUpdater(std::bind(&Player::Update, this));
	}

	void Player::OnCollision(const Collision* collision)
	{
		if (collision->m_ColliderA->m_ColliderName == "leader-bullet" ||
			collision->m_ColliderA->m_ColliderName == "pawn-bullet")
		{
			((SimpleFollower*)collision->m_ColliderA)->DestroySelf();
			m_Lives--;
		}
		else if (collision->m_ColliderB->m_ColliderName == "leader-bullet" ||
			collision->m_ColliderB->m_ColliderName == "pawn-bullet")
		{
			((SimpleFollower*)collision->m_ColliderB)->DestroySelf();
			m_Lives--;
		}

		LOG("Lives: ");
		LOG(m_Lives);
	}

	void Player::Update()
	{
		if (!m_CanShoot)
		{
			if (m_ShootDelay > 0)
			{
				m_ShootDelay -= Game::s_DeltaTime;
				return;
			}
			else
			{
				m_CanShoot = true;
				m_ShootDelay = m_ShootDelayReset;
			}
		}
	}

	void Player::Shoot()
	{
		if (!m_CanShoot) return;

		m_CanShoot = false;
		m_ShootDelay = m_ShootDelayReset;

		SimpleBulletController* bullet = new SimpleBulletController(Vec2Int(GetX(), GetY() - GetHeight()), Vec2Int(GetX(), -GetHeight()), 4);
		bullet->Configure(20, Rect(0, 0, Game::s_Width, Game::s_Height));
		bullet->AddSprite("player-bullet", Vec2Int(GetX(), GetY() - GetHeight()), Vec2Int(16, 16), 1);
	}
}