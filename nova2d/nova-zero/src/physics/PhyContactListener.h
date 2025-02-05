#pragma once
#include "Box2D/Box2D.h"
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <vector>
#include <functional>
#include "PhyBase.h"

namespace novazero
{
	namespace physics
	{
		struct PhyCollision
		{
			PhyBase* m_ContactA = nullptr;
			std::string m_GroupA = "none";

			PhyBase* m_ContactB = nullptr;
			std::string m_GroupB = "none";

			std::function<void(PhyBase* a, PhyBase* b)> f_OnEnter = nullptr;
			std::function<void(PhyBase* a, PhyBase* b)> f_OnExit = nullptr;
		};

		class PhyContactListener : public b2ContactListener
		{

		private:

			std::vector<PhyCollision*> m_Collisions;
			std::vector<PhyCollision*> m_CollisionsErasers;

		public:

			PhyContactListener();

			PhyCollision* AddCollision(PhyBase* contactA, PhyBase* contactB,
				std::function<void(PhyBase* a, PhyBase* b)> onEnter,
				std::function<void(PhyBase* a, PhyBase* b)> onExit);

			PhyCollision* AddCollision(const std::string& groupA, const std::string& groupB,
				std::function<void(PhyBase* a, PhyBase* b)> onEnter,
				std::function<void(PhyBase* a, PhyBase* b)> onExit);

			void RemoveCollision(PhyCollision* collision);
			void Clean();

			void BeginContact(b2Contact* contact) override;
			void EndContact(b2Contact* contact) override;	

		};
	}
}