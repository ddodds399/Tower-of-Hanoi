#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include<SDL2\SDL.h>

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void Update();
	bool IsClosed();
	bool IsUpPressed();
	bool IsRightPressed();
	bool IsLeftPressed();
	bool IsDownPressed();
	bool IsDPressed();
	bool IsAPressed();
	bool IsRPressed();
	bool IsSpacePressed();
	bool IsKeyDown();
	//Used to set the value for a keyboard key press
	inline void SetKeyDown(bool tf) { m_keyDown = tf;  }
	virtual ~Display();
	void ResetAll();

protected:
private:
	Display(const Display& other) {}
	void operator=(const Display& other) {}

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed;
	bool m_isFullscreen;
	bool m_upPress;
	bool m_rightPress;
	bool m_leftPress;
	bool m_downPress;
	bool m_spacePress;
	bool m_aPress;
	bool m_dPress;
	bool m_rPress;
	bool m_keyDown;
};


#endif // DISPLAY_H