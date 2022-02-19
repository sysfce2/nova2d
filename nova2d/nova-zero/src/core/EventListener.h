#pragma once
#include "../core/TypeDefs.h"
#include <vector>
#include "../core/Deleteable.h"

namespace novazero
{
	namespace core
	{
		class EventListener : public Deleteable
		{

		private:
			
			std::vector<SDL_Keycode> m_KeyCodes;
			std::vector<f_ConditionalFunction> m_KeysConditions;
			std::vector<f_VoidFunction> m_KeysEvents;

			std::vector<SDL_Keycode> m_KeyCodes1;
			std::vector<f_ConditionalFunction> m_KeysConditions1;
			std::vector<f_VoidFunction> m_KeysEvents1;

			std::vector<int> m_JoysticksIDs;
			std::vector<int> m_JoystickButtons;
			std::vector<f_JoyStickConditionalFunction> m_JoysticksConditions;
			std::vector<f_VoidFunction> m_JoysticksEvents;

			std::vector<int> m_JoysticksIDs1;
			std::vector<int> m_JoystickButtons1;
			std::vector<f_JoyStickConditionalFunction> m_JoysticksConditions1;
			std::vector<f_VoidFunction> m_JoysticksEvents1;

			std::vector<int> m_JoystickAxisIDs;
			std::vector<JoystickAxis> m_JoystickAxisAxis;
			std::vector<f_JoyStickAxisConditionalFunction> m_JoystickAxisConditions;
			std::vector<f_FloatPassFunction> m_JoystickAxisEvents;

			unsigned int m_ID = 0;

		public:
			
			EventListener();
			~EventListener();

			void StartEventListener();
			void EndEventListener();

			void EventStep();

			void AddKeysEventListener(SDL_KeyCode key, f_ConditionalFunction conditionalFunction, f_VoidFunction executeFunction);
			void RemoveEventListener(SDL_KeyCode key);

			void AddKeysEventListener1(SDL_KeyCode key, f_ConditionalFunction conditionalFunction, f_VoidFunction executeFunction);
			void RemoveEventListener1(SDL_KeyCode key);

			void AddJoyEventListener(int joystickID, int button, f_JoyStickConditionalFunction conditionalFunction, f_VoidFunction executeFunction);
			void RemoveJoyEventListener(int joystickID, int button);

			void AddJoyEventListener1(int joystickID, int button, f_JoyStickConditionalFunction conditionalFunction, f_VoidFunction executeFunction);
			void RemoveJoyEventListener1(int joystickID, int button);

			void AddJoyAxisEventListener(int joystickID, JoystickAxis axis, f_JoyStickAxisConditionalFunction conditionalFunction, f_FloatPassFunction executeFunction);
			void RemoveJoyAxisEventListener(int joystickID);

			bool operator==(const EventListener& other);

			void DestroySelf();

		};
	}
}