#include "PhySprite.h"
#include "../core/Game.h"

namespace novazero
{
	namespace physics
	{
		using namespace graphics;
		using namespace core;

		PhySprite::PhySprite(const std::string& assetName, Vec2 position, Vec2 size, 
			unsigned char layer, Vec2Int displaySize, std::string colliderName, const std::string& collisionGroup)
			: Sprite(assetName, position, displaySize, layer), PhyBase(colliderName, this, collisionGroup)
		{
			m_ID = n2dGameGetID();
			m_DeleteName = "phySprite_" + tostring(m_ID);
			m_PhySize = size;

			auto uID = n2dAddUpdater(PhySprite::Update, this);
			Deleteable::m_CleanUpdaters.push_back(uID);
		}

		unsigned int PhySprite::GetPhyID() const
		{
			return m_ID;
		}

		void PhySprite::EnableRotation(bool val)
		{
			if (m_Body)
			{
				m_Body->SetFixedRotation(!val);
			}
		}

		void PhySprite::Update()
		{
			if (m_Body && GetSprite())
			{
				b2Vec2 bodyPos = m_Body->GetPosition();
				Vec2 screenPos = Vec2::MetersToPixels(bodyPos);

				GetSprite()->SetWorldCenterPosition(screenPos);

				if(m_AutoRotate)
					GetSprite()->SetAngle(n2dRadToDeg(m_Body->GetAngle()));
			}
		}

		int PhySprite::GetWidth() const
		{
			return (int)m_PhySize.x;
		}

		int PhySprite::GetHeight() const
		{
			return (int)m_PhySize.y;
		}

		void PhySprite::ConfigurePhysicsPolygon(bool staticBody, std::vector<Vec2> shapeVertices, const int vertexCount, float density, float friction)
		{
			if (vertexCount < 3)
			{
				LOG(LVL_NFE, "Polygon shape body not created. Too few points of shape.", __FILE__, __LINE__);
				return;
			}

			if (vertexCount > 8)
			{
				LOG(LVL_NFE, "Polygon shape body not created. Too many points of shape (change max points).", __FILE__, __LINE__);
				return;
			}

			if (vertexCount != (int)shapeVertices.size())
			{
				LOG(LVL_NFE, "Polygon shape body not created. Vertex count and number of vertices not equal.", __FILE__, __LINE__);
				return;
			}

			b2World* world = n2dCurrentPhyWorld();

			if (!world)
			{
				LOG(LVL_NFE, "Tryinig to create a physics body when no physics world is present.", __FILE__, __LINE__);
				return;
			}

			if (!GetPhySprite())
			{
				LOG(LVL_NFE, "Tryinig to create a physics sprite with no sprite.", __FILE__, __LINE__);
				return;
			}

			if (m_Body)
				world->DestroyBody(m_Body);

			b2BodyDef bodyDef;
			bodyDef.userData = reinterpret_cast<void*>((PhyBase*)this);
			bodyDef.type = staticBody ? b2_staticBody : b2_dynamicBody;
			
			b2Vec2 pos = Vec2::PixelsToMeters(Vec2(GetX(), GetY()));
			bodyDef.position.Set(pos.x, pos.y);

			m_Body = world->CreateBody(&bodyDef);

			b2Vec2* vertices = new b2Vec2[shapeVertices.size()];

			if (!vertices)
			{
				LOG(LVL_NFE, "Could not free vertices for polygon shape.", __FILE__, __LINE__);
				return;
			}

			for (size_t i = 0; i < shapeVertices.size(); i++)
			{
				const b2Vec2 v = b2Vec2(shapeVertices[i].x * PHYSICS_SCALE, shapeVertices[i].y * PHYSICS_SCALE);
				vertices[i] = v;
			}

			b2PolygonShape shape;
			shape.Set(vertices, (int32)shapeVertices.size());

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = density;
			fixtureDef.friction = friction;

			m_Body->CreateFixture(&fixtureDef);
			m_Body->SetSleepingAllowed(false);

			delete[] vertices;

		}

		void PhySprite::ConfigurePhysicsRect(bool staticBody, float density, float friction)
		{
			b2World* world = n2dCurrentPhyWorld();

			if (!world)
			{
				LOG(LVL_NFE, "Tryinig to create a physics body when no physics world is present.", __FILE__, __LINE__);
				return;
			}

			if (!GetPhySprite())
			{
				LOG(LVL_NFE, "Tryinig to create a physics sprite with no sprite.", __FILE__, __LINE__);
				return;
			}

			if (m_Body)
				world->DestroyBody(m_Body);

			b2BodyDef bodyDef;
			bodyDef.userData = reinterpret_cast<void*>((PhyBase*)this);
			bodyDef.type = staticBody ? b2_staticBody : b2_dynamicBody;

			b2Vec2 pos = Vec2::PixelsToMeters(GetPosition());
			bodyDef.position.Set(pos.x, pos.y);

			m_Body = world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			b2Vec2 boxSize = Vec2::PixelsToMeters(Vec2((float)GetWidth() / 2.f, (float)GetHeight() / 2.f));
			m_BodySize = boxSize;
			shape.SetAsBox(boxSize.x, boxSize.y);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = density;
			fixtureDef.friction = friction;

			m_Body->CreateFixture(&fixtureDef);
			m_Body->SetSleepingAllowed(false);

			m_CircleShape = false;
			m_RectShape = true;

		}

		void PhySprite::ConfigurePhysicsCircle(bool staticBody, float radiusInPixels, float density, float friction)
		{
			b2World* world = n2dCurrentPhyWorld();

			if (!world)
			{
				LOG(LVL_NFE, "Tryinig to create a physics body when no physics world is present.", __FILE__, __LINE__);
				return;
			}

			if (!GetPhySprite())
			{
				LOG(LVL_NFE, "Tryinig to create a physics sprite with no sprite.", __FILE__, __LINE__);
				return;
			}

			if (m_Body)
				world->DestroyBody(m_Body);

			b2BodyDef bodyDef;
			bodyDef.userData = reinterpret_cast<void*>((PhyBase*)this);			
			bodyDef.type = staticBody ? b2_staticBody : b2_dynamicBody;

			b2Vec2 pos = Vec2::PixelsToMeters(Vec2(GetX() + GetWidth() / 2, GetY() + GetHeight() / 2));
			bodyDef.position.Set(pos.x, pos.y);

			m_Body = world->CreateBody(&bodyDef);

			b2CircleShape shape;
			shape.m_radius = n2dPixelsToMeters(radiusInPixels) * PHYSICS_SCALE;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = density;
			fixtureDef.friction = friction;

			m_Body->CreateFixture(&fixtureDef);
			m_Body->SetSleepingAllowed(false);

			m_CircleShape = true;
			m_RectShape = false;

		}

		void PhySprite::ConfigurePhysicsSensorCircle(bool staticBody, float radiusInPixels, float density, float friction)
		{
			b2World* world = n2dCurrentPhyWorld();

			if (!world)
			{
				LOG(LVL_NFE, "Tryinig to create a physics body when no physics world is present.", __FILE__, __LINE__);
				return;
			}

			if (m_Body)
				world->DestroyBody(m_Body);
			
			b2BodyDef bodyDef;
			bodyDef.userData = reinterpret_cast<void*>((PhyBase*)this);
			bodyDef.type = staticBody ? b2_staticBody : b2_dynamicBody;

			b2Vec2 pos = Vec2::PixelsToMeters(Vec2(GetX() + GetWidth() / 2, GetY() + GetHeight() / 2));
			bodyDef.position.Set(pos.x, pos.y);

			m_Body = world->CreateBody(&bodyDef);

			b2CircleShape shape;
			shape.m_radius = n2dPixelsToMeters(radiusInPixels) * PHYSICS_SCALE;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.isSensor = true;
			fixtureDef.density = density;
			fixtureDef.friction = friction;

			m_Body->CreateFixture(&fixtureDef);
			m_Body->SetSleepingAllowed(false);

			m_CircleShape = true;
			m_RectShape = false;

		}

		void PhySprite::ApplyForce(Vec2 forceDirNormalized, float magnitude)
		{
			if (!m_Body) return;

			forceDirNormalized.scale(magnitude * n2dTimeScale);
			b2Vec2 force = b2Vec2(forceDirNormalized.x, forceDirNormalized.y);
			m_Body->ApplyForce(force, m_Body->GetWorldCenter(), true);
		}

		void PhySprite::ApplyForce(float force, Directions direction)
		{
			if (m_Body)
			{
				b2Vec2 forceDirection;
				switch (direction)
				{
				case Directions::Up:
					forceDirection = m_Body->GetWorldVector(b2Vec2(0, -1));
					break;
				case Directions::Down:
					forceDirection = m_Body->GetWorldVector(b2Vec2(0, 1));
					break;
				case Directions::Right:
					forceDirection = m_Body->GetWorldVector(b2Vec2(1, 0));
					break;
				case Directions::Left:
					forceDirection = m_Body->GetWorldVector(b2Vec2(-1, 0));
					break;
				}

				Vec2 dir = Vec2(forceDirection.x, forceDirection.y);
				ApplyForce(dir, force);
				
			}
		}

		void PhySprite::ApplyAngularForce(float force)
		{
			if (m_Body)
			{
				m_Body->ApplyAngularImpulse(force, true);
			}
		}

		void PhySprite::SetScale(float factor)
		{
			return;
			// TODO: scaling does not work

			if (m_Body)
			{
				b2Fixture* f = m_Body->GetFixtureList();
				m_Body->DestroyFixture(f);

				if (m_RectShape && GetPhySprite())
				{
					b2PolygonShape shape;
					shape.SetAsBox(GetPhySprite()->GetWidth() / 2 * PHYSICS_SCALE * factor, 
						GetPhySprite()->GetHeight() / 2 * PHYSICS_SCALE * factor);
					
					m_Body->CreateFixture(&shape, m_Density);
					m_Body->GetFixtureList()->SetDensity(m_Density);
					m_Body->GetFixtureList()->SetFriction(m_Friction);
					m_Body->GetFixtureList()->SetRestitution(m_Restitution);
				}
				else if (m_CircleShape)
				{
					b2CircleShape shape;
					shape.m_radius = m_Radius * PHYSICS_SCALE * factor;
					
					m_Body->CreateFixture(&shape, m_Density);
					m_Body->GetFixtureList()->SetDensity(m_Density);
					m_Body->GetFixtureList()->SetFriction(m_Friction);
					m_Body->GetFixtureList()->SetRestitution(m_Restitution);
				}
				else
				{
					LOG(LVL_NON_FATAL_ERROR, "Cannot scale polygon shaped bodies.", __FILE__, __LINE__);
					return;
				}

				if (GetSprite())
					GetSprite()->SetScale(factor);
			}
		}

		void PhySprite::SetPosition(Vec2 position)
		{
			if (m_Body)
			{
				b2Vec2 meterPos = Vec2::PixelsToMeters(position);
				if (m_CircleShape)
				{
					m_Body->SetTransform(meterPos, m_Body->GetAngle());
				}
				else
				{
					m_Body->SetTransform(b2Vec2(meterPos.x + GetWidth() / 2, meterPos.y + GetHeight() / 2), m_Body->GetAngle());
				}
			}

			Sprite::SetPosition(position);
		}

		void PhySprite::DestroySelf()
		{
			if(m_Body)
				Game::s_SceneManager->s_PhyCleaners.push_back(m_Body);
	
			CleanUpdaters();
			Sprite::DestroySelf();
		}
	
	}
}