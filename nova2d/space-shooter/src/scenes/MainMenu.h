#include "core/Scene.h"
#include "maths/Vec2Int.h"
#include "core/EventListener.h"

namespace spaceshooter
{
	using namespace novazero::core;
	using namespace novazero::input;

	class MainMenu :
		public Scene
	{

	private:

		Sprite* title = nullptr;

	public:

		MainMenu(std::string sceneName)
			: Scene(sceneName)
		{

		};

		~MainMenu() { };

		void Update() override
		{

		}

		void Start() override
		{
			n2dScoreSet(0);

			title = new Sprite("title", Vec2Int(Game::s_Width / 2, Game::s_Height / 2),
				Vec2Int(256, 128), 0);

			title->SetPosition(Vec2Int(title->GetX() - title->GetWidth() / 2, title->GetY() - title->GetHeight() / 2));
		
			auto startListening = new auto([=] {
				StartListening();
			});

			Timer* t = new Timer(1000, false, *startListening);
		}

		void StartListening()
		{
			n2dAddKeyDownListener(SDLK_SPACE, MainMenu::OnSpace, this);
			n2dAddKeyDownListener(SDLK_ESCAPE, MainMenu::OnEscape, this);
		}

		void OnSpace()
		{
			n2dRemoveKeyDownListener(SDLK_SPACE);
			n2dRemoveKeyDownListener(SDLK_ESCAPE);
			n2dSceneChange("level1");
		}

		void OnEscape()
		{
			Game::EndGame(0);
		}

		void End() override
		{
			delete title;
		}

	};
}