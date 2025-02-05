#include "EventListener.h"
#include "Game.h"

namespace novazero
{
	namespace core
	{
		EventListener::EventListener()
			: Deleteable("eventListener"), m_ID(0)
		{
			m_ID = n2dGameGetID();

			m_DeleteName = "eventListener_" + std::to_string(m_ID);

			StartEventListener();

		}

		EventListener::~EventListener()
		{
			
		}

		void EventListener::DestroySelf()
		{
			EndEventListener();
			SetDeleted(true);
		}

		void EventListener::StartEventListener()
		{
 			auto cleanID = n2dAddUpdater(EventListener::EventStep, this);
			m_CleanUpdaters.push_back(cleanID);
		}

		void EventListener::EndEventListener()
		{
			CleanUpdaters();
		}

		void EventListener::EventStep()
		{
			for (size_t i = 0; i < m_KeysConditions.size(); i++)
			{
				if (m_KeysConditions[i](m_KeyCodes[i]) == true)
				{
					m_KeysEvents[i]();
				}
			}

			for (size_t i = 0; i < m_KeysConditions1.size(); i++)
			{
				if (m_KeysConditions1[i](m_KeyCodes1[i]) == true)
				{
					m_KeysEvents1[i]();
				}
			}

			for (size_t i = 0; i < m_JoysticksConditions.size(); i++)
			{
				if (m_JoysticksConditions[i](m_JoysticksIDs[i], m_JoystickButtons[i]) == true)
				{
					m_JoysticksEvents[i]();
				}
			}

			for (size_t i = 0; i < m_JoystickAxisConditions.size(); i++)
			{
				m_JoystickAxisEvents[i](m_JoystickAxisConditions[i](m_JoystickAxisIDs[i], m_JoystickAxisAxis[i]));
			}

		}

		void EventListener::AddKeysEventListener(SDL_KeyCode key, std::function<bool(SDL_Keycode)> conditionalFunction, std::function<void()> executeFunction)
		{
			if (m_KeyCodes.end() != std::find(m_KeyCodes.begin(), m_KeyCodes.end(), key))
			{
				return;
			}

			m_KeyCodes.push_back(key);
			m_KeysConditions.push_back(conditionalFunction);
			m_KeysEvents.push_back(executeFunction);
		}

		void EventListener::AddKeysEventListener1(SDL_KeyCode key, std::function<bool(SDL_Keycode)> conditionalFunction, std::function<void()> executeFunction)
		{
			if (m_KeyCodes1.end() != std::find(m_KeyCodes1.begin(), m_KeyCodes1.end(), key))
			{
				return;
			}

			m_KeyCodes1.push_back(key);
			m_KeysConditions1.push_back(conditionalFunction);
			m_KeysEvents1.push_back(executeFunction);
		}
		
		void EventListener::RemoveEventListener(SDL_KeyCode key)
		{
			int idx = -1;
			for (size_t i = 0; i < m_KeyCodes.size(); i++)
			{
				if (m_KeyCodes[i] == key)
				{
					idx = i;
					break;
				}
			}

			if (idx == -1) return;

			m_KeyCodes.erase(m_KeyCodes.begin() + idx);
			m_KeysConditions.erase(m_KeysConditions.begin() + idx);
			m_KeysEvents.erase(m_KeysEvents.begin() + idx);

		}

		void EventListener::RemoveEventListener1(SDL_KeyCode key)
		{
			int idx = -1;
			for (size_t i = 0; i < m_KeyCodes1.size(); i++)
			{
				if (m_KeyCodes1[i] == key)
				{
					idx = i;
					break;
				}
			}

			if (idx == -1) return;

			m_KeyCodes1.erase(m_KeyCodes1.begin() + idx);
			m_KeysConditions1.erase(m_KeysConditions1.begin() + idx);
			m_KeysEvents1.erase(m_KeysEvents1.begin() + idx);

		}

		void EventListener::AddJoyEventListener(int joystickID, int button, std::function<bool(int, int)> conditionalFunction, std::function<void()> executeFunction)
		{
			if (m_JoysticksIDs.end() != std::find(m_JoysticksIDs.begin(), m_JoysticksIDs.end(), joystickID))
			{
				return;
			}

			m_JoysticksIDs.push_back(joystickID);
			m_JoystickButtons.push_back(button);
			m_JoysticksConditions.push_back(conditionalFunction);
			m_JoysticksEvents.push_back(executeFunction);
		}

		void EventListener::AddJoyEventListener1(int joystickID, int button, std::function<bool(int, int)> conditionalFunction, std::function<void()> executeFunction)
		{
			if (m_JoysticksIDs1.end() != std::find(m_JoysticksIDs1.begin(), m_JoysticksIDs1.end(), joystickID))
			{
				return;
			}

			m_JoysticksIDs1.push_back(joystickID);
			m_JoystickButtons1.push_back(button);
			m_JoysticksConditions1.push_back(conditionalFunction);
			m_JoysticksEvents1.push_back(executeFunction);
		}

		void EventListener::RemoveJoyEventListener(int joystickID, int button)
		{
			int idx = -1;
			for (size_t i = 0; i < m_JoysticksIDs.size(); i++)
			{
				if (m_JoysticksIDs[i] == joystickID &&
					m_JoystickButtons[i] == button)
				{
					idx = i;
					break;
				}
			}

			if (idx == -1) return;

			m_JoysticksIDs.erase(m_JoysticksIDs.begin() + idx);
			m_JoystickButtons.erase(m_JoystickButtons.begin() + idx);
			m_JoysticksConditions.erase(m_JoysticksConditions.begin() + idx);
			m_JoysticksEvents.erase(m_JoysticksEvents.begin() + idx);

		}

		void EventListener::RemoveJoyEventListener1(int joystickID, int button)
		{
			int idx = -1;
			for (size_t i = 0; i < m_JoysticksIDs1.size(); i++)
			{
				if (m_JoysticksIDs1[i] == joystickID &&
					m_JoystickButtons1[i] == button)
				{
					idx = i;
					break;
				}
			}

			if (idx == -1) return;

			m_JoysticksIDs1.erase(m_JoysticksIDs1.begin() + idx);
			m_JoystickButtons1.erase(m_JoystickButtons1.begin() + idx);
			m_JoysticksConditions1.erase(m_JoysticksConditions1.begin() + idx);
			m_JoysticksEvents1.erase(m_JoysticksEvents1.begin() + idx);

		}

		void EventListener::AddJoyAxisEventListener(int joystickID, int axis, std::function<bool(int, int)> conditionalFunction, std::function<void(float)> executeFunction)
		{
			if (m_JoysticksIDs.end() != std::find(m_JoysticksIDs.begin(), m_JoysticksIDs.end(), joystickID))
			{
				return;
			}

			m_JoystickAxisIDs.push_back(joystickID);
			m_JoystickAxisAxis.push_back(axis);
			m_JoystickAxisConditions.push_back(conditionalFunction);
			m_JoystickAxisEvents.push_back(executeFunction);
		}

		void EventListener::RemoveJoyAxisEventListener(int joystickID)
		{
			int idx = -1;
			for (size_t i = 0; i < m_JoystickAxisIDs.size(); i++)
			{
				if (m_JoystickAxisIDs[i] == joystickID)
				{
					idx = i;
					break;
				}
			}

			if (idx == -1) return;

			m_JoystickAxisIDs.erase(m_JoystickAxisIDs.begin() + idx);
			m_JoystickAxisAxis.erase(m_JoystickAxisAxis.begin() + idx);
			m_JoystickAxisConditions.erase(m_JoystickAxisConditions.begin() + idx);
			m_JoystickAxisEvents.erase(m_JoystickAxisEvents.begin() + idx);

		}

		bool EventListener::operator==(const EventListener& other)
		{
			return m_ID == other.m_ID;
		}
	}
}