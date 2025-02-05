#pragma once
#include "EventListener.h"
#include "../maths/Vec2.h"
#include <functional>
#include <vector>
#include <map>
#include <string>
#include "Box2D/Box2D.h"
#include "SDL.h"
#include "../core/Environment.h"
#include "../debug/PhysicsDebug.h"
#include "../physics/PhyContactListener.h"
#include "../particles/ParticleDestructionListener.h"
#include "../physics/PhyContactFilter.h"
#include "../particles/ParticleSystem.h"

namespace novazero
{
	namespace core
	{
		using namespace novazero::maths;
		using namespace novazero::debug;
		using namespace novazero::particles;

		class Scene : 
			public EventListener
		{
		
		private:

			std::vector<void*> m_CleanUpObjects;
			
		private:

			// Physics
			float* m_TimeStep = new float;
			b2World* m_World = nullptr;
			b2Vec2* m_Gravity = nullptr;

			PhyContactListener* m_ContactListener = nullptr;
			PhyContactFilter* m_ContactFilter = nullptr;
			ParticleDestructionListener* m_ParticleDestructionListener = nullptr;

		public:

			Scene(const std::string& sceneName);

			bool m_PhysicsEnabled = false;
			bool m_DebugDraw = false;
			bool m_Started = false;
			std::string m_SceneName = "";

			static Environment* s_Environment;

			void EnablePhysics(bool enabled, Vec2 gravity = Vec2(0,0), float innerPadding = 0.f, bool walls = true);
			b2World* GetWorld() const;

			PhyContactListener* GetContactListener() const
			{
				return m_ContactListener;
			}

			PhyContactFilter* GetContactFilter() const
			{
				return m_ContactFilter;
			}
			
			void PhysicsStep();
			void PhysicsEnableDebug(bool isDebug);

			void EnableLights(Uint8 worldLightIntensity);

			virtual void Start() = 0;
			void Restart();
			void Clean();

			virtual void End() = 0;
			virtual void Update() = 0;

			void UpdateParticles();

			void AddObjectToCleanUp(void* obj);
			void CleanUp();

		public:

			static PhysicsDebug* s_PhysicsDrawer;

			static std::map<std::string, ParticleSystem*> s_ParticleSystems;
			
			static ParticleSystem* AddParticleSystem(const std::string& assetName, Vec2Int size,
				const std::string& systemName, int32 maxParticles, float particleRadius,
				unsigned char layer);

			static ParticleSystem* GetParticleSystem(const std::string& systemName);
			static void RemoveParticleSystem(const std::string& systemName);
			static int32 GetParticleCount();
		};
	}
}