#include <ui/window.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>

#if !defined(__APPLE__)
	#define HAVE_M_PI
#endif
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

const uint16_t KEY_MAP[Window::KEY_COUNT] =
{
	0x0,
	SDL_SCANCODE_UP,
	SDL_SCANCODE_DOWN,
	SDL_SCANCODE_LEFT,
	SDL_SCANCODE_RIGHT,
	SDL_SCANCODE_W,
	SDL_SCANCODE_A,
	SDL_SCANCODE_S,
	SDL_SCANCODE_D,
	SDL_SCANCODE_Q,
	SDL_SCANCODE_E,
	SDL_SCANCODE_T,
	SDL_SCANCODE_U,
	SDL_SCANCODE_Y,
	SDL_SCANCODE_RETURN,
	SDL_SCANCODE_ESCAPE,
	SDL_SCANCODE_SPACE
};

void SDL_ERROR(const char* msg) {
	printf("%s:\n%s\n", msg, SDL_GetError());
}

Window* Window::Instance = nullptr;

Window::Window()
{
    m_Keyboard = nullptr;
    m_MouseMode = MOUSE_MODE::DISABLED;
}

Window* Window::CreateInstance(const char* title, unsigned int width, unsigned int height)
{
    Instance = new Window();
    if(Instance != nullptr)
    {
        if(!Instance->Initialize(title, width, height))
        {
            delete Instance;
            Instance = nullptr;
        }
    }
    return Instance;
}

void Window::DeleteInstance()
{
    delete Instance;
    Instance = nullptr;
}

bool Window::Initialize(const char* title, unsigned int width, unsigned int height)
{
    bool status = true;
    #define SDL_ASSERT(b, e) if(!(b)) { SDL_ERROR(e); status = false; }

	SDL_ASSERT(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "SDL initialization failure");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if(status)
    {
        m_hWindow = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );
        SDL_ASSERT(m_hWindow != NULL, "SDL window creation failure");
    }

    if(status)
    {
        m_hContext = SDL_GL_CreateContext(reinterpret_cast<SDL_Window*>(m_hWindow));
    	SDL_ASSERT(m_hContext != NULL, "SDL OpenGL context creation failure");
    }

    if(status)
    {
    	m_Keyboard = SDL_GetKeyboardState(nullptr);
    }

    return status;
}

Window* Window::GetInstance()
{
    return Instance;
}

Window::~Window()
{
	if (m_hContext != nullptr) {
		SDL_GL_DeleteContext(m_hContext);
	}

	if (m_hWindow != nullptr) {
		SDL_DestroyWindow(reinterpret_cast<SDL_Window*>(m_hWindow));
	}

	SDL_Quit();
}

void Window::Update()
{
	SDL_GL_SwapWindow(reinterpret_cast<SDL_Window*>(m_hWindow));
}

bool Window::PollEvent(Event& e)
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				e.type = EVENT_QUIT;
				return true;
			}
			default: break;
		}
	}
	
	return false;
}

void Window::GetMouseState(MouseState& state)
{
	uint32_t buttons = 0;

	switch(m_MouseMode)
	{
		case MOUSE_MODE::ABSOLUTE:
		{
			buttons = SDL_GetMouseState(&state.x_offset, &state.y_offset);
			break;
		}
		case MOUSE_MODE::RELATIVE:
		{
			buttons = SDL_GetRelativeMouseState(&state.x_offset, &state.y_offset);
			break;
		}
		default:
		{
			break;
		}
	}

	state.l_button = (buttons & SDL_BUTTON_LMASK) != 0;
	state.m_button = (buttons & SDL_BUTTON_MMASK) != 0;
	state.r_button = (buttons & SDL_BUTTON_RMASK) != 0;
}

Window::MOUSE_MODE Window::GetMouseMode()
{
	return m_MouseMode;
}

bool Window::SetMouseMode(MOUSE_MODE mode)
{
	bool status = true;

	switch(mode)
	{
		case MOUSE_MODE::DISABLED:
		{
			m_MouseMode = MOUSE_MODE::DISABLED;
			if(SDL_GetRelativeMouseMode() == SDL_TRUE)
			{
				status = (SDL_SetRelativeMouseMode(SDL_FALSE) == 0);
			}
			break;
		}
		case MOUSE_MODE::ABSOLUTE:
		{
			m_MouseMode = MOUSE_MODE::ABSOLUTE;
			if(SDL_GetRelativeMouseMode() == SDL_TRUE)
			{
				status = (SDL_SetRelativeMouseMode(SDL_FALSE) == 0);
			}
			break;
		}
		case MOUSE_MODE::RELATIVE:
		{
			m_MouseMode = MOUSE_MODE::RELATIVE;
			if(SDL_GetRelativeMouseMode() != SDL_TRUE)
			{
				status = (SDL_SetRelativeMouseMode(SDL_TRUE) == 0);
			}
			break;
		}
		default:
		{
			status = false;
			printf("error: invalid mouse mode (0x%hhX)\n", mode);
			break;
		}
	}

	return status;
}

uint32_t Window::GetTime()
{
	return SDL_GetTicks();
}

bool Window::GetKeyState(KEY_CODE code)
{
	return (m_Keyboard[KEY_MAP[code]] != 0);
}
