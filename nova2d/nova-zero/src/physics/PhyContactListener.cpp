#pragma once
#include "PhyContactListener.h"
#include "../physics/PhyBase.h"
#include "../logging/logging.h"
#include "../core/Game.h"

namespace novazero
{
	namespace physics
	{
		using namespace novazero::physics;
		using namespace novazero::core;

		PhyContactListener::PhyContactListener() {};

		PhyCollision* PhyContactListener::AddCollision(PhyBase* self, PhyBase* other, 
			std::function<void(PhyBase* a, PhyBase* b)> onEnter,
			std::function<void(PhyBase* a, PhyBase* b)> onExit)
		{

			if (!self || !other)
			{
				LOG(LVL_NFE, "Could not add collision. A or B PhyBase is NULL.", __FILE__, __LINE__);
				return nullptr;
			}

			unsigned int id = n2dGameGetID();

			PhyCollision* collision = new PhyCollision();
			collision->m_ContactA = self;
			collision->m_ContactB = other;
			collision->f_OnEnter = onEnter;
			collision->f_OnExit = onExit;

			if (n2dDebugVerbose)
				LOGS("Added collision: " + self->GetColliderName() + "[" + tostring(self->GetPhyID()) + "]" +
					" : " + other->GetColliderName() + "[" + tostring(other->GetPhyID()) + "]");

			m_Collisions.push_back(collision);

			return collision;

		}

		PhyCollision* PhyContactListener::AddCollision(const std::string& groupA, const std::string& groupB,
			std::function<void(PhyBase* a, PhyBase* b)> onEnter,
			std::function<void(PhyBase* a, PhyBase* b)> onExit)
		{
			if (groupA == "none" || groupB == "none")
			{
				LOG(LVL_NFE, "Could not add collision. A or B PhyBase group is NONE.", __FILE__, __LINE__);
				return nullptr;
			}

			unsigned int id = n2dGameGetID();

			PhyCollision* collision = new PhyCollision();
			collision->m_GroupA = groupA;
			collision->m_GroupB = groupB;
			collision->f_OnEnter = onEnter;
			collision->f_OnExit = onExit;

			if (n2dDebugVerbose)
				LOGS("Added group collision: " + groupA + " <--> " + groupB);

			m_Collisions.push_back(collision);

			return collision;
		}

		void PhyContactListener::Clean()
		{
			auto it = m_CollisionsErasers.begin();
			while(it != m_CollisionsErasers.end())
			{
				it = m_CollisionsErasers.erase(it);
			}
			m_CollisionsErasers.clear();
		}

		void PhyContactListener::RemoveCollision(PhyCollision* collision)
		{
			m_CollisionsErasers.push_back(collision);
		}

		void PhyContactListener::BeginContact(b2Contact* contact)
		{
			PhyBase* self = reinterpret_cast<PhyBase*>(contact->GetFixtureA()->GetBody()->GetUserData());
			PhyBase* other = reinterpret_cast<PhyBase*>(contact->GetFixtureB()->GetBody()->GetUserData());

			if (!self || !other) return;

			bool skipGroupCollision = (self->GetCollisionGroup() == "none" || other->GetCollisionGroup() == "none");

			if (n2dDebugVerbose)
			{
				if (!skipGroupCollision)
				{
					std::string s = self->GetCollisionGroup();
					std::string o = other->GetCollisionGroup();
					LOGS("Enter: [ " + s + " :: " + o + "]");
				}
			}

			unsigned int selfID = self->GetPhyID();
			unsigned int otherID = other->GetPhyID();

			for (auto it = m_Collisions.begin(); it != m_Collisions.end(); it++)
			{
				PhyCollision& collision = *(*it);

				bool nullContacts = !collision.m_ContactA || !collision.m_ContactB;
				
				if (!skipGroupCollision && (collision.m_GroupA == self->GetCollisionGroup() && collision.m_GroupB == other->GetCollisionGroup()))
				{
					if (collision.f_OnEnter)
					{
						collision.f_OnEnter(self, other);
					}
				}
				else if (!skipGroupCollision && (collision.m_GroupA == other->GetCollisionGroup() && collision.m_GroupB == self->GetCollisionGroup()))
				{
					if (collision.f_OnEnter)
					{
						collision.f_OnEnter(other, self);
					}
				}
				else if (!nullContacts && collision.m_ContactA->GetPhyID() == selfID && collision.m_ContactB->GetPhyID() == otherID)
				{
					if (collision.f_OnEnter)
					{
						collision.f_OnEnter(self, other);
					}
				}
				else if (!nullContacts && collision.m_ContactA->GetPhyID() == otherID && collision.m_ContactB->GetPhyID() == selfID)
				{
					if (collision.f_OnEnter)
					{
						collision.f_OnEnter(other, self);
					}
				}
			}

			if (n2dDebugVerbose)
				LOGS("Contact: " + self->GetColliderName() + "[" + tostring(selfID) + "] : " +
					other->GetColliderName() + "[" + tostring(otherID) + "]");
			
		}

		void PhyContactListener::EndContact(b2Contact* contact)
		{
			PhyBase* self = reinterpret_cast<PhyBase*>(contact->GetFixtureA()->GetBody()->GetUserData());
			PhyBase* other = reinterpret_cast<PhyBase*>(contact->GetFixtureB()->GetBody()->GetUserData());

			if (!self || !other) return;

			bool skipGroupCollision = (self->GetCollisionGroup() == "none" || other->GetCollisionGroup() == "none");
			
			if (n2dDebugVerbose)
			{
				if (!skipGroupCollision)
				{
					std::string s = self->GetCollisionGroup();
					std::string o = other->GetCollisionGroup();
					LOGS("Exit: [ " + s + " :: " + o + "]");
				}
			}

			unsigned int selfID = self->GetPhyID();
			unsigned int otherID = other->GetPhyID();

			for (auto it = m_Collisions.begin(); it != m_Collisions.end(); it++)
			{
				PhyCollision& collision = *(*it);

				bool nullContacts = !collision.m_ContactA || !collision.m_ContactB;

				if (!skipGroupCollision && (collision.m_GroupA == self->GetCollisionGroup() && collision.m_GroupB == other->GetCollisionGroup()))
				{
					if (collision.f_OnExit)
					{
						collision.f_OnExit(self, other);
					}
				}
				else if (!skipGroupCollision && (collision.m_GroupA == other->GetCollisionGroup() && collision.m_GroupB == self->GetCollisionGroup()))
				{
					if (collision.f_OnExit)
					{
						collision.f_OnExit(other, self);
					}
				}
				else if (!nullContacts && collision.m_ContactA->GetPhyID() == selfID && collision.m_ContactB->GetPhyID() == otherID)
				{
					if (collision.f_OnExit)
					{
						collision.f_OnExit(self, other);
					}
				}
				else if (!nullContacts && collision.m_ContactA->GetPhyID() == otherID && collision.m_ContactB->GetPhyID() == selfID)
				{
					if (collision.f_OnExit)
					{
						collision.f_OnExit(other, self);
					}
				}
			}

			if (n2dDebugVerbose)
				LOGS("Contact: " + self->GetColliderName() + "[" + tostring(selfID) + "] : " +
					other->GetColliderName() + "[" + tostring(otherID) + "]");
		}

	}
}