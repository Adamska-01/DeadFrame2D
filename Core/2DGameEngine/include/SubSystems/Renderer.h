#pragma once


struct SDL_Window;
struct SDL_Renderer;


class Renderer
{
private:
	Renderer();

	
	static Renderer* Instance;


	SDL_Window* Window{ nullptr };
	
	SDL_Renderer* renderer{ nullptr };

public:
	inline static Renderer* GetInstance() { return Instance = (Instance != nullptr) ? Instance : new Renderer(); }


	void SetDisplayColour(int r, int g, int b, int a);
	
	void Update();
	
	void ClearAndPresent();
	
	void Delay(float dt);
	
	void Destroy();

	inline SDL_Renderer* GetRenderer() { return renderer; }
};