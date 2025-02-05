#include "Window.h"
#include "../logging/Logging.h"
#include "../core/Game.h"

#ifdef NOVA_EMSCRIPTEN
// For emscripten, instead of using glad we use its built-in support for OpenGL:
#include <GL/gl.h>
#else
#include "../gui/glad/glad.h"
#endif

#include "../gui/imgui/imgui.h"
#include "../gui/imgui/imgui_impl_sdl.h"
#include "../gui/imgui/imgui_impl_sdlrenderer.h"

namespace novazero
{
	namespace graphics
	{
		using namespace maths;

		Window::Window(const Vec2Int screenSize, const char* title)
			: m_Title(title)
		{
			m_Width = (int)floor(screenSize.x);
			m_Height = (int)floor(screenSize.y);

			Init();
		}

		void Window::DestroySelf()
		{
			SDL_GL_DeleteContext(m_Window);

			if (m_Icon)
				SDL_FreeSurface(m_Icon);

			m_Icon = NULL;
			SDL_DestroyWindow(m_Window);
		}

		void Window::Init()
		{
			using namespace logging;
			
#ifdef NOVA_EMSCRIPTEN
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0) {
				std::string err = SDL_GetError();
				LOG(LVL_FE, "Unable to initialize SDL: " + err, __FILE__, __LINE__);
				return;
		}
#else
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0) {
				std::string err = SDL_GetError();
				LOG(LVL_FE, "Unable to initialize SDL: " + err, __FILE__, __LINE__);
				return;
			}
#endif

#ifdef NOVA_EMSCRIPTEN
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

			m_Window = SDL_CreateWindow(
				m_Title,
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				m_Width,
				m_Height,
				SDL_WINDOW_OPENGL
			);

			if (m_Window)
			{
				m_GlContext = SDL_GL_CreateContext(m_Window);

				if (!m_GlContext)
				{
					std::string err = SDL_GetError();
					LOG(LVL_FE, "Could not create Open GL context: " + err, __FILE__, __LINE__);
					return;
				}

				SDL_GL_MakeCurrent(m_Window, m_GlContext);
				SDL_GL_SetSwapInterval(1);

#ifndef NOVA_EMSCRIPTEN

				if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
				{
					LOG(LVL_FATAL_ERROR, "Could not initialize glad.", __FILE__, __LINE__);
					return;
				}
#endif
			}
			else
			{
				std::string err = SDL_GetError();
				LOG(LVL_FATAL_ERROR, "Could not create window: " + err, __FILE__, __LINE__);
				return;
			}

		}

		void Window::SetupImGUI(SDL_Renderer* renderer)
		{
			if (!m_Window)
			{
				LOG(LVL_NFE, "Could not start GUI.", __FILE__, __LINE__);
				return;
			}

			glViewport(0, 0, m_Width, m_Height);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGui_ImplSDL2_InitForSDLRenderer(m_Window, renderer);
			ImGui_ImplSDLRenderer_Init(renderer);

			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontDefault();
			io.Fonts->Build();

		}

		SDL_Window* Window::GetWindow() const
		{
			return m_Window;
		}
	}
}