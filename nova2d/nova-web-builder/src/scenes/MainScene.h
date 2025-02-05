#pragma once
#include "core/Scene.h"
#include <map>

namespace webbuilder
{
	using namespace novazero::core;

	class MainScene :
		public Scene
	{

	private:

		unsigned int m_GUICleanID = 0;

		bool m_CreateMakefile = true;
		const int MAKE_FILE_MAX = 8192;
		char m_MakefileBuffer[8192] = {};

		bool m_Optomize = true;
		bool m_UseWebGL2 = true;
		bool m_UseSDL2 = true;
		bool m_UseSDLTTF = true;
		bool m_UseSDLImage = true;
		bool m_UseSDLPNG = true;
		bool m_UseOGGandMP3 = true;
		bool m_UseSDL2Mixer = true;
		bool m_PackageResFolder = true;

		bool m_ShowSRCDirectroy = false;
		char m_SrcDirectory[500] = {};
		char m_ResDirectory[500] = "res\0";
		char m_NovaDirectory[500] = "..\\..\\nova-zero\\src\0";
		char m_Box2dDirectory[500] = "..\\..\\box2d\0";

		std::map<std::string, bool*> m_SrcDirectories;

	public:

		MainScene(const std::string& sceneName);

		void Start() override;

		void Update() override;

		void DrawGUI();

		void UpdateSRCDirectory(const std::string& srcDir);
		int GetDirectories(const char* dir);

		void UpdateMakefile();
		void SaveMakefile();

		void End() override;

		void DestroySelf();
	};
}