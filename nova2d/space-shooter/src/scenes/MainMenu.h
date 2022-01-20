#include "core/Scene.h"
#include "maths/Vec2Int.h"

namespace spaceshooter
{
	using namespace novazero::core;

	class MainMenu :
		public Scene, public EventListener
	{

	private:

		Sprite* title = nullptr;

	public:

		MainMenu() { };
		~MainMenu() { };

		void Update() override
		{

		}

		void Start() override
		{
			title = new Sprite("title", Vec2Int(Game::s_Width / 2, Game::s_Height / 2),
				Vec2Int(128, 32), 0);

			title->SetPosition(Vec2Int(title->GetX() - title->GetWidth() / 2, title->GetY() - title->GetHeight() / 2));
		
			// Link so events get triggered
			SceneManager::AddUpdater(std::bind(&MainMenu::EventStep, this));
			AddKeysEventListener(SDLK_SPACE, &InputHandler::IsKeyDown, std::bind(&MainMenu::OnSpace, this));
		}

		void OnSpace()
		{
			Game::s_SceneManager->ChangeScene("game");
		}

		void End() override
		{
			delete title;
		}

	};
}