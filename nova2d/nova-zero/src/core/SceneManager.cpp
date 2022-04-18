#include "SceneManager.h"
#include "../graphics/Renderer.h"
#include <algorithm>
#include "Game.h"
#include "box2d/b2_draw.h"

namespace novazero
{
	namespace core
	{
		std::map<unsigned int, std::function<void()>> SceneManager::s_Updaters;
		std::map<unsigned int, bool> SceneManager::s_UpdaterErasers;
		std::map<unsigned int, std::function<void()>> SceneManager::s_UpdatersToAdd;

		std::map<unsigned int, std::function<void()>> SceneManager::s_PersistentUpdaters;
		std::map<unsigned int, bool> SceneManager::s_PersistentUpdaterErasers;
		std::map<unsigned int, std::function<void()>> SceneManager::s_PersistentUpdatersToAdd;

		std::map<unsigned int, std::function<void()>> SceneManager::s_GUIUpdaters;
		std::map<unsigned int, bool> SceneManager::s_GUIUpdaterErasers;
		std::map<unsigned int, std::function<void()>> SceneManager::s_GUIUpdatersToAdd;

		std::map<unsigned int, Deleteable*> SceneManager::s_Deleteables;

		ReferenceManager* SceneManager::s_ReferenceManager;
		CollisionManager* SceneManager::s_CollisionManager;
		GraverManager* SceneManager::s_GraverManager;
		TweenManager* SceneManager::s_TweenManager;
		TimeEffectorManager* SceneManager::s_TimeEffectorManager;
		std::map<std::string, Timeline*> SceneManager::s_Timelines;

		SceneManager::SceneManager()
		{
			s_CollisionManager = new CollisionManager();
			s_ReferenceManager = new ReferenceManager();
			s_GraverManager = new GraverManager();
			s_TweenManager = new TweenManager();
			s_TimeEffectorManager = new TimeEffectorManager();
			
			// Default timeline
			AddTimelineEvent("main", nullptr);
		}
		
		void SceneManager::ConfigureFirstScene(const std::string& sceneName)
		{
			Scene* loadScene = GetScene(sceneName);
			if (loadScene == nullptr)
			{
				LOG(LVL_FATAL_ERROR, "Could not start. No first scene: " + sceneName);
				return;
			}

			m_LastSceneName = sceneName;
			m_CurrentSceneName = sceneName;
			m_CurrentScene = loadScene;
			m_CurrentScene->m_Started = true;
			m_CurrentScene->Start();
		}

		void SceneManager::AddScene(const std::string& sceneName, Scene* scene)
		{
			m_Scenes[sceneName] = scene;
		}

		void SceneManager::RemoveScene(const std::string& sceneName)
		{
			m_Scenes.erase(sceneName);
		}

		Scene* SceneManager::GetScene(const std::string& sceneName)
		{
			if (m_Scenes.find(sceneName) == m_Scenes.end()) return nullptr;
			return m_Scenes[sceneName];
		}

		Scene* SceneManager::GetCurrentScene()
		{
			return m_CurrentScene;
		}

		void SceneManager::ChangeScene(const std::string& sceneName)
		{
			Scene* loadScene = GetScene(sceneName);
			if (loadScene == nullptr)
			{
				LOG(LVL_NON_FATAL_ERROR, "Cannot find scene: " + sceneName);
				return;
			}

			if(m_CurrentSceneName != m_CurrentScene->m_SceneName)
				m_LastSceneName = m_CurrentSceneName;
			
			m_CurrentScene->Clean();
			m_CurrentScene->CleanUp();
			m_CurrentScene->End();

			CleanUpdaters();
			s_CollisionManager->ClearColliders();
			Renderer::s_DrawLayers->ClearSprites();
			s_Deleteables.clear();
			s_GraverManager->ClearGravers();
			s_TweenManager->ClearTweens();
			s_TimeEffectorManager->ClearEffectors();
			s_TimeEffectorManager->ClearEffected();
			CAMERA->Reset();

			m_CurrentScene = loadScene;
			
			if(!m_CurrentScene->m_Started) m_CurrentScene->Restart();
			m_CurrentScene->m_Started = true;

			LOG(LVL_INFO, "Starting Scene [ " + m_CurrentScene->m_SceneName + " ]");

			m_CurrentSceneName = m_CurrentScene->m_SceneName;
			m_CurrentScene->Start();

			if (Game::s_DebugOverlay)
			{
				Game::ConfigureDebugOverlay(true);
				Game::ConfigureDebugPosition(Vec2Int(32, 32));
			}

		}

		void SceneManager::CleanUpdaters()
		{
			// mark all updaters to be removed
			std::map<unsigned int, std::function<void()>>::iterator it = s_Updaters.begin();
			while (it != s_Updaters.end())
			{
				s_UpdaterErasers[it->first] = true;
				it++;
			}
		}

		b2World* SceneManager::GetCurrentWorld() const
		{
			if(m_CurrentScene)
				return m_CurrentScene->GetWorld();
			return nullptr;
		}

		void SceneManager::Clean()
		{
			std::map<unsigned int, Deleteable*>::iterator it = s_Deleteables.begin();
			std::vector<unsigned int> removeIDs;
			for (it; it != s_Deleteables.end(); it++)
			{
				if (it->second->NeedsDestruction())
				{
					it->second->SetDeleted(true);
				}

				if (it->second->IsDeleted() == 1)
				{
					removeIDs.push_back(it->first);
					delete it->second;
				}
			}

			for (size_t i = 0; i < removeIDs.size(); i++)
			{
				s_Deleteables.erase(removeIDs[i]);
			}
		}
		
		void SceneManager::Update()
		{
			if (m_Destroyed) return;

			if (Game::s_TimeScale == 0.f)
			{
				ProcessPersistentUpdaters();
				return;
			}

			std::map<std::string, Timeline*>::iterator it = s_Timelines.begin();
			while (it != s_Timelines.end())
			{
				it->second->Update();
				it++;
			}

			if (m_CurrentScene == nullptr && Game::s_Running)
			{
				LOG(LVL_FATAL_ERROR, "NO Scene loaded. Exiting.");
				Game::EndGame(ERR_NO_SCENE);
				return;
			}
			else if (!Game::s_Running)
			{
				return;
			}

			m_CurrentScene->PhysicsStep(); // first physics
			m_CurrentScene->Update();

			ProcessPersistentUpdaters();
			ProcessUpdaters();

			s_CollisionManager->Update();
			s_GraverManager->Update();
			s_TweenManager->Update();
			s_TimeEffectorManager->Update();
		}

		void SceneManager::ProcessUpdaters()
		{
			// Add new updaters
			std::map<unsigned int, std::function<void()>>::iterator itA = s_UpdatersToAdd.begin();
			while (itA != s_UpdatersToAdd.end())
			{
				s_Updaters[itA->first] = itA->second;
				s_UpdaterErasers[itA->first] = false;
				itA++;
			}

			s_UpdatersToAdd.clear();

			// Remove any removed updaters
			std::map<unsigned int, bool>::iterator it = s_UpdaterErasers.begin();
			while (it != s_UpdaterErasers.end())
			{
				if (it->second)
				{
					s_Updaters.erase(it->first);
				}

				it++;
			}

			s_UpdaterErasers.clear();

			// Update
			std::map<unsigned int, std::function<void()>>::iterator it2 = s_Updaters.begin();
			for (; it2 != s_Updaters.end(); ++it2)
			{
				if (it2->second)
				{
					it2->second();
				}
 			}
		}

		void SceneManager::ProcessPersistentUpdaters()
		{
			// Add new updaters
			std::map<unsigned int, std::function<void()>>::iterator itA = s_PersistentUpdatersToAdd.begin();
			while (itA != s_PersistentUpdatersToAdd.end())
			{
				s_PersistentUpdaters[itA->first] = itA->second;
				s_PersistentUpdaterErasers[itA->first] = false;
				itA++;
			}

			s_PersistentUpdatersToAdd.clear();

			// Remove any removed updaters
			std::map<unsigned int, bool>::iterator it = s_PersistentUpdaterErasers.begin();
			while (it != s_PersistentUpdaterErasers.end())
			{
				if (it->second)
				{
					s_PersistentUpdaters.erase(it->first);
				}

				it++;
			}

			s_PersistentUpdaterErasers.clear();

			// Update
			std::map<unsigned int, std::function<void()>>::iterator it2 = s_PersistentUpdaters.begin();
			for (; it2 != s_PersistentUpdaters.end(); ++it2)
			{
				if (it2->second)
				{
					it2->second();
				}
			}
		}

		void SceneManager::RenderGUI()
		{
			// Add new updaters
			std::map<unsigned int, std::function<void()>>::iterator itA = s_GUIUpdatersToAdd.begin();
			while (itA != s_GUIUpdatersToAdd.end())
			{
				s_GUIUpdaters[itA->first] = itA->second;
				s_GUIUpdaterErasers[itA->first] = false;
				itA++;
			}

			s_GUIUpdatersToAdd.clear();

			// Remove any removed updaters
			std::map<unsigned int, bool>::iterator it = s_GUIUpdaterErasers.begin();
			while (it != s_GUIUpdaterErasers.end())
			{
				if (it->second)
				{
					s_GUIUpdaters.erase(it->first);
				}

				it++;
			}

			s_GUIUpdaterErasers.clear();

			// Update
			std::map<unsigned int, std::function<void()>>::iterator it2 = s_GUIUpdaters.begin();
			for (; it2 != s_GUIUpdaters.end(); ++it2)
			{
				if (it2->second)
				{
					it2->second();
				}
			}
		}

		void SceneManager::RemoveUpdater(unsigned int id)
		{
			s_UpdaterErasers[id] = true;
		}

		unsigned int SceneManager::AddUpdater(std::function<void()> updater)
		{
			unsigned int id = n2dGameGetID();
			s_UpdatersToAdd[id] = updater;
			return id;
		}

		void SceneManager::RemoveGUIUpdater(unsigned int id)
		{
			s_GUIUpdaterErasers[id] = true;
		}

		unsigned int SceneManager::AddGUIUpdater(std::function<void()> updater)
		{
			unsigned int id = n2dGameGetID();
			s_GUIUpdatersToAdd[id] = updater;
			return id;
		}

		void SceneManager::RemovePersistentUpdater(unsigned int id)
		{
			s_PersistentUpdaterErasers[id] = true;
		}

		unsigned int SceneManager::AddPersistentUpdater(std::function<void()> persistentUpdater)
		{
			unsigned int id = n2dGameGetID();
			s_PersistentUpdatersToAdd[id] = persistentUpdater;
			return id;
		}

		void SceneManager::RemoveDeleteable(unsigned int id)
		{
			s_Deleteables.erase(id);
		}

		unsigned int SceneManager::AddDeleteable(Deleteable* object)
		{
			unsigned int id = n2dGameGetID();
			s_Deleteables[id] = object;
			return id;
		}

		void SceneManager::AddTimelineEvent(const std::string& timelineName, TimelineEvent* timelineEvent)
		{
			if (s_Timelines.find(timelineName) == s_Timelines.end())
			{
				Timeline* timeline = new Timeline(timelineName);
				s_Timelines[timelineName] = timeline;
				
				if(timelineEvent != nullptr)
					s_Timelines[timelineName]->AddEvent(timelineEvent);
			}
			else
			{
				if (timelineEvent != nullptr)
					s_Timelines[timelineName]->AddEvent(timelineEvent);
			}
		}

		void SceneManager::StartAndResetTimeline(const std::string& timelineName)
		{
			if (s_Timelines.find(timelineName) == s_Timelines.end())
			{
				LOG(LVL_W, timelineName + " does not exists. Cannot start/restart.");
				return;
			}
			else
			{
				s_Timelines[timelineName]->ResetToStartEvent(true);
			}
		}

		void SceneManager::CleanTimeline(const std::string& timelineName)
		{
			std::map<std::string, Timeline*>::iterator it = s_Timelines.begin();
			while (it != s_Timelines.end())
			{
				if (it->first == timelineName)
				{
					it->second->DestroySelf();
					it++;
					return;
				}
				it++;
			}
		}

		void SceneManager::CleanTimelines()
		{
			std::map<std::string, Timeline*>::iterator it = s_Timelines.begin();
			while (it != s_Timelines.end())
			{
				it->second->DestroySelf();
				it++;
			}
		}

		void SceneManager::DestroySelf()
		{
			if (m_Destroyed) return;
			m_Destroyed = true;

			if (s_ReferenceManager)
				delete s_ReferenceManager;

			if (s_CollisionManager)
				delete s_CollisionManager;

			if (s_GraverManager)
			{
				s_GraverManager->ClearGravers();
				delete s_GraverManager;
			}

			if (s_TweenManager)
			{
				s_TweenManager->ClearTweens();
				delete s_TweenManager;
			}

			if (s_TimeEffectorManager)
			{
				s_TimeEffectorManager->ClearEffectors();
				s_TimeEffectorManager->ClearEffected();
				delete s_TimeEffectorManager;
			}

			CleanTimelines();

		}
	}
}