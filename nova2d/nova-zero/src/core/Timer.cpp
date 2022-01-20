#include "Timer.h"
#include "Game.h"

namespace novazero
{
	namespace core
	{
		Timer::Timer(const float delayMS, const bool loop, f_VoidFunction endDelayFunc)
		{
			m_DelayMax = delayMS;
			m_Delay = m_DelayMax;
			m_Loop = loop;

			m_OnFinish = endDelayFunc;

			n2dAddUpdater(Timer::Update, this);
		}

		Timer::~Timer()
		{
			n2dRemoveUpdater(Timer::Update, this);
		}

		void Timer::Update()
		{
			if (!m_Alive) return;

			if (m_Delay < 0)
			{
				if (m_Loop)
				{
					m_Alive = true;
					m_Delay = m_DelayMax;
				}
				else
				{
					m_Alive = false;
				}

				m_OnFinish();

			}
			else
			{
				m_Delay = (float)(m_Delay - Game::s_DeltaTime);
			}
		}
	}
}