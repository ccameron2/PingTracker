#include "App.h"

//#ifdef WINDOWS
//#include "SDL3/SDL_main.h"
//#endif
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

// Main code
int main(int argc, char* argv[])
{
{
	std::unique_ptr<App> app = std::make_unique<App>();

	while(!app->Update());

    return 0;
}
#ifdef _DEBUG
_CrtDumpMemoryLeaks();
#endif
}