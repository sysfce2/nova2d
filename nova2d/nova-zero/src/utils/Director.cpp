#include "Director.h"
#include "../core/Game.h"

namespace novazero
{
	namespace utils
	{
		using namespace graphics;
		using namespace maths;
		using namespace core;

		// ----------------------------
		BYTE Director::s_CurrentPage = 0;
		std::vector<DirectorPage*> Director::s_Pages;

		Director::Director(Vec2Int position) : Drawable(), Deleteable("director")
		{
			m_ID = n2dGameGetID();
			m_Layer = 255;

			n2dAddDrawable(this, m_Layer);

			for(int i = 0; i < MAX_DIRECTOR_PAGES; i++)
				s_Pages.push_back(new DirectorPage());

			m_Position = position;

			int x = position.x;
			int y = position.y + 10;

			m_Background = new DrawRect("a20-blue", "white", true,
				Rect(x, y, m_Width, m_Height), 2, m_Layer);

			int padding = 10;
 			m_Title = new Text("font1", "Director", "white", Rect((float)x + padding, (float)y + padding - 45.f, 100.f, 30.f), m_Layer);

			m_ScrollTime = new ScrollSelect("Time Scale", 60, "white", (float)m_Width - m_Title->GetWidth(), 20.f, 0.01f, 4.f, &Game::s_TimeScaleMemory,
				Rect((float)x + padding, (float)y + padding + 15.f, (float)m_Width + padding, 30.f), "light-blue", "white", m_Layer, true);

			m_ScrollTime->Select(true, "bright-blue", "white");
			m_ScrollTime->SetValueColor("purple");

			m_Visible = true;
			Toggle();

			auto cleanID = n2dAddUpdaterPersistent(Director::Update, this);
			m_CleanUpdaters.push_back(cleanID);

		}

		void Director::Update()
		{
			if (m_Destroyed) return;
			if (!IsEnabled()) return;

			if (n2dIsKeyUp(SDLK_UP) && n2dIsKeyUp(SDLK_DOWN))
			{
				m_SelectionLock = false;
			}

			if (m_SelectionLock) return;

			const size_t leftCount = s_Pages[s_CurrentPage]->m_LeftStack.size();
			const size_t rightCount = s_Pages[s_CurrentPage]->m_RightStack.size();

			bool selectNew = false;
			ScrollSelect* currentSelected = m_ScrollTime;

			if (n2dIsKeyDown(SDLK_UP))
			{
				m_SelectionLock = true;

				if (m_CurrentSelected != -1)
				{
					if (m_LeftSelectedStack)
					{
						currentSelected = s_Pages[m_CurrentSelected]->m_LeftStack.at(m_CurrentSelected);
					}
					else
					{
						currentSelected = s_Pages[m_CurrentSelected]->m_RightStack.at(m_CurrentSelected);
					}
				}			

				if (m_LeftSelectedStack)
				{
					if (m_CurrentSelected > -1)
					{
						m_CurrentSelected--;
						selectNew = true;
					}
					else
					{
						return;
					}
				}
				else
				{
					if (m_CurrentSelected <= 0)
					{
						m_LeftSelectedStack = true;
						m_CurrentSelected = leftCount - 1;						
						selectNew = true;
					}
					else
					{
						m_CurrentSelected--;
						selectNew = true;
					}
				}
			}
			else if (n2dIsKeyDown(SDLK_DOWN))
			{
				m_SelectionLock = true;

				if (m_CurrentSelected != -1)
				{
					if (m_LeftSelectedStack)
					{
						currentSelected = s_Pages[m_CurrentSelected]->m_LeftStack.at(m_CurrentSelected);
					}
					else
					{
						currentSelected = s_Pages[m_CurrentSelected]->m_RightStack.at(m_CurrentSelected);
					}
				}

				if (m_LeftSelectedStack)
				{
					if (leftCount != 0)
					{
						if (m_CurrentSelected == -1 && leftCount > 0)
						{
							m_CurrentSelected = 0;
							selectNew = true;
						}
						else if (m_CurrentSelected < leftCount - 1)
						{
							m_CurrentSelected++;
							selectNew = true;
						}
						else if(rightCount > 0)
						{
							m_CurrentSelected = 0;
							m_LeftSelectedStack = false;
							selectNew = true;
						}
					}
					else
					{
						m_LeftSelectedStack = false;
						m_CurrentSelected = 0;
						selectNew = rightCount > 0;
					}
				}
				else
				{
					if (rightCount != 0)
					{
						if (m_CurrentSelected < rightCount - 1)
						{
							m_CurrentSelected++;
							selectNew = true;
						}
					}
				}
			}

			if (selectNew)
			{
				ChangeSelection(currentSelected);
			}
		}

		void Director::ChangeSelection(ScrollSelect* oldSelected)
		{
			oldSelected->Select(false, "light-blue", "white");

			if (m_CurrentSelected == -1)
			{
				m_ScrollTime->Select(true, "bright-blue", "white");
				return;
			}

			if (m_LeftSelectedStack)
			{
				s_Pages[m_CurrentSelected]->m_LeftStack.at(m_CurrentSelected)->Select(true, "bright-blue", "white");
			}
			else
			{
				s_Pages[m_CurrentSelected]->m_RightStack.at(m_CurrentSelected)->Select(true, "bright-blue", "white");
			}
		}

		void Director::Toggle()
		{
			m_Visible = !m_Visible;

			m_Background->SetVisible(m_Visible);
			m_Title->SetVisible(m_Visible);
			m_ScrollTime->SetVisible(m_Visible);

			m_Background->SetEnabled(m_Visible);
			m_Title->SetEnabled(m_Visible);
			m_ScrollTime->SetEnabled(m_Visible);

			for (size_t i = 0; i < s_Pages.size(); i++)
			{
				for (size_t l = 0; l < s_Pages[i]->m_LeftStack.size(); l++)
				{
					s_Pages[i]->m_LeftStack[l]->SetVisible(m_Visible);
					s_Pages[i]->m_LeftStack[l]->SetEnabled(m_Visible);
				}

				for (size_t r = 0; r < s_Pages[i]->m_RightStack.size(); r++)
				{
					s_Pages[i]->m_RightStack[r]->SetVisible(m_Visible);
					s_Pages[i]->m_RightStack[r]->SetEnabled(m_Visible);
				}
			}

			n2dPauseGame(m_Visible);

		}

		void Director::Draw(float oX, float oY)
		{

		}

		void Director::AddToStack(bool left, BYTE page, std::string labelText, int labelWidth, float inOrDecreaseby, float max, float* refVal)
		{
			if (page < 0) page = 0;
			if (page > MAX_DIRECTOR_PAGES) page = MAX_DIRECTOR_PAGES;

			const int currentLeftSize = s_Pages.at(page)->m_LeftStack.size();
			const int currentRightSize = s_Pages.at(page)->m_RightStack.size();
			
			if (left && currentLeftSize >= 5)
			{
				if (page != MAX_DIRECTOR_PAGES)
				{
					page++;
					LOG(LVL_I, "Could not place " + labelText + 
						" on Director Page [" + tostring(page) + "]. Added to next page.");
				}
				else
				{
					LOG(LVL_W, "Ran out of space to place: " + labelText + " on Director LEFT stack");
					return;
				}
			}
			else if(!left && currentRightSize)
			{
				if (page != MAX_DIRECTOR_PAGES)
				{
					page++;
					LOG(LVL_I, "Could not place " + labelText +
						" on Director Page [" + tostring(page) + "]. Added to next page.");
				}
				else
				{
					LOG(LVL_W, "Ran out of space to place: " + labelText + " on Director RIGHT stack");
					return;
				}
			}

			if (left)
			{
				float x = s_LeftStackX;
				float y = s_StackStartY + currentLeftSize * 50.f;

				ScrollSelect* selector = new ScrollSelect(labelText, labelWidth, "white", 150.f, 14.f, inOrDecreaseby, max, refVal,
					Rect(x, y, 180.f, 30.f), "light-blue", "white", 255, true);
				
				selector->SetValueColor("purple");
				selector->SetVisible(false);
				selector->SetEnabled(false);
				
				s_Pages.at(page)->m_LeftStack.push_back(selector);
			}
			else
			{		
				float x = s_RightStackX;
				float y = s_StackStartY + currentRightSize * 50.f;

				ScrollSelect* selector = new ScrollSelect(labelText, labelWidth, "white", 150.f, 14.f, inOrDecreaseby, max, refVal,
					Rect(x, y, 180.f, 30.f), "light-blue", "white", 255, true);
				
				selector->SetValueColor("purple");
				selector->SetVisible(false);
				selector->SetEnabled(false);

				s_Pages.at(page)->m_RightStack.push_back(selector);
			}
		}

		void Director::DestroySelf()
		{
			if (m_Destroyed) return;
			m_Destroyed = true;

			CleanUpdaters();

			n2dRemoveDrawable(m_ID, m_Layer);

			m_Background->DestroySelf();
			m_Title->DestroySelf();

			SetDeleted(true);
		}
	}
}