#ifndef UI_KIT_WINDOW_HPP
#define UI_KIT_WINDOW_HPP

#include <cstdint>

class Window
{
private:
    static Window* Instance;

public:
    enum KEY_CODE
    {
        KEY_UNKNOWN = 0x00,
        KEY_UP      = 0x01,
        KEY_DOWN    = 0x02,
        KEY_LEFT    = 0x03,
        KEY_RIGHT   = 0x04,
        KEY_W       = 0x05,
        KEY_A       = 0x06,
        KEY_S       = 0x07,
        KEY_D       = 0x08,
        KEY_Q       = 0x09,
        KEY_E       = 0x0A,
        KEY_T       = 0x0B,
        KEY_U       = 0x0C,
        KEY_Y       = 0x0D,
        KEY_RETURN  = 0x0E,
        KEY_ESCAPE  = 0x0F,
        KEY_SPACE   = 0x10,
        KEY_COUNT   = 0x11
    };

    enum EVENT_TYPE
    {
        EVENT_UNKNOWN = 0x0,
        EVENT_QUIT    = 0x1
    };

    struct Event
    {
        uint8_t type;
    };

    struct MouseState
    {
        int  x_offset;
        int  y_offset;
        bool l_button;
        bool m_button;
        bool r_button;
    };

    enum MOUSE_MODE : uint8_t
    {
        DISABLED = 0,
        ABSOLUTE = 1,
        RELATIVE = 2
    };

private:
    Window();
    ~Window();

    bool Initialize(const char* title, unsigned int width, unsigned int height);

private:
    void* m_hWindow;
    void* m_hContext;
    
    MOUSE_MODE m_MouseMode;
    const uint8_t* m_Keyboard;

public:
    static Window* CreateInstance(const char* title, unsigned int width, unsigned int height);
    static void DeleteInstance();

    static Window* GetInstance();

public:
    void Update();
	bool PollEvent(Event& e);
	
	MOUSE_MODE GetMouseMode();
	bool       SetMouseMode(MOUSE_MODE mode);

    void GetMouseState(MouseState& state);

	uint32_t GetTime();
	bool GetKeyState(KEY_CODE code);
};

#endif // UI_KIT_WINDOW_HPP