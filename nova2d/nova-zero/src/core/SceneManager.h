#pragma once
#include "Scene.h"
#include "../utils/ReferenceManager.h"
#include <map>
#include <string>
#include <vector>
#include "../core/Deleteable.h"
#include "../utils/TweenManager.h"
#include "../utils/timeline/Timeline.h"

namespace novazero
{

	namespace core
	{
		using namespace utils;
		using namespace utils::timeline;
		using namespace physics;

		class SceneManager
		{
		
		private:
		
			std::map<std::string, Scene*> m_Scenes;
			Scene* m_CurrentScene = nullptr;
			bool m_Destroyed = false;

		public:

			std::string m_LastSceneName = "none";
			std::string m_CurrentSceneName = "none";

		public:
		
			SceneManager();

			void ConfigureFirstScene(const std::string& sceneName);
						
			void AddScene(const std::string& sceneName, Scene* scene);
			void RemoveScene(const std::string& sceneName);
			Scene* GetScene(const std::string& sceneName);
			void ChangeScene(const std::string& sceneName);

			Scene* GetCurrentScene();

			void Update();
			void DoPhysics();
			void ProcessUpdaters();
			void ProcessPersistentUpdaters();
			void RenderGUI();
			void CleanUpdaters();
			void Clean();

			b2World* GetCurrentWorld() const;

			static void AddTimelineEvent(const std::string& timelineName, TimelineEvent* timelineEvent);
			static void StartAndResetTimeline(const std::string& timelineName);
			static void CleanTimelines();
			static void CleanTimeline(const std::string& timelineName);

			void DestroySelf();

			static ReferenceManager* s_ReferenceManager;
			static TweenManager* s_TweenManager;
			static std::map<std::string, Timeline*> s_Timelines;

			static std::map<unsigned int, std::function<void()>> s_GUIUpdaters;
			static std::map<unsigned int, bool> s_GUIUpdaterErasers;
			static std::map<unsigned int, std::function<void()>> s_GUIUpdatersToAdd;
			static unsigned int AddGUIUpdater(std::function<void()> updater);
			static void RemoveGUIUpdater(unsigned int id);
			static unsigned int GetGUIUpdaterCount()
			{
				return (unsigned int)s_Updaters.size();
			};

			static std::map<unsigned int, std::function<void()>> s_Updaters;
			static std::map<unsigned int, bool> s_UpdaterErasers;
			static std::map<unsigned int, std::function<void()>> s_UpdatersToAdd;
			static unsigned int AddUpdater(std::function<void()> updater);
			static void RemoveUpdater(unsigned int id);
			static unsigned int GetUpdaterCount() 
			{ 
				return (unsigned int)s_Updaters.size(); 
			};

			static std::map<unsigned int, std::function<void()>> s_PersistentUpdaters;
			static std::map<unsigned int, bool> s_PersistentUpdaterErasers;
			static std::map<unsigned int, std::function<void()>> s_PersistentUpdatersToAdd;
			static unsigned int AddPersistentUpdater(std::function<void()> persistentUpdater);
			static void RemovePersistentUpdater(unsigned int id);
			static unsigned int GetPersistentUpdaterCount()
			{
				return (unsigned int)s_PersistentUpdaters.size();
			};

			static std::map<unsigned int, Deleteable*> s_Deleteables;
			static unsigned int AddDeleteable(Deleteable* o);
			static void RemoveDeleteable(unsigned int id);
			static unsigned int GetDeleteableCount()
			{
				return (unsigned int)s_Deleteables.size();
			};

			static std::vector<b2Body*> s_PhyCleaners;
		};
	}
}