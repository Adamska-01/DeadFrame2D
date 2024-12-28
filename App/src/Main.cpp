#include "Application.h"


int main(int argc, char** argv)
{
	auto App = new Application();

	App->Update();

	delete App;
	App = nullptr;
	
	return 0;
}