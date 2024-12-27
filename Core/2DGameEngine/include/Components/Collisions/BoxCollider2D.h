#pragma once
#include <SDL.h>
#include <SubSystems/Renderer.h>


class BoxCollider2D
{
private:
	SDL_Rect Box;

	SDL_Rect Buffer;


public:
	inline SDL_Rect GetBox() { return Box; }

	inline void SetBuffer(int x, int y, int w, int h) { Buffer = { x, y, w, h }; } 

	inline void SetBox(int x, int y, int w, int h) 
	{
		Box = 
		{
			x - Buffer.x,
			y - Buffer.y,
			w - Buffer.w,
			h - Buffer.h
		};
	}

	//For debug
	inline void DrawBox(Uint8 r, Uint8 g, Uint8 b, Uint8 a) 
	{
		SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), r, g, b, a);
		SDL_RenderDrawRect(Renderer::GetInstance()->GetRenderer(), &Box);
	}
};