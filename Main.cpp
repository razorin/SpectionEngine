#include <stdlib.h>
#include "Application.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#ifdef _MSC_VER
#	pragma comment (lib, "opengl32.lib")
#	pragma comment (lib, "glu32.lib")
#	ifdef _WIN64
#		ifdef _DEBUG
#			pragma comment(lib, "MathGeoLib/libx64/Debug/MathGeoLib.lib")
#			pragma comment(lib, "Glew/lib/Release/Win32/glew32.lib")
#			pragma comment( lib, "DevIL/lib/x86/Release/DevIL.lib" )
#			pragma comment( lib, "DevIL/lib/x86/Release/ILU.lib" )
#			pragma comment( lib, "DevIL/lib/x86/Release/ILUT.lib" )
#		else // RELEASE
#			pragma comment(lib, "MathGeoLib/libx64/Release/MathGeoLib.lib")
#			pragma comment(lib, "Glew/lib/Release/Win32/glew32.lib")
#			pragma comment( lib, "DevIL/lib/x86/Release/DevIL.lib" )
#			pragma comment( lib, "DevIL/lib/x86/Release/ILU.lib" )
#			pragma comment( lib, "DevIL/lib/x86/Release/ILUT.lib" )
#		endif // _DEBUG
#	else // WIN32
#		ifdef _DEBUG
#			pragma comment(lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib")
#			pragma comment(lib, "Glew/lib/Release/Win32/glew32.lib")
#			pragma comment( lib, "DevIL/lib/x86/Release/DevIL.lib" )
#			pragma comment( lib, "DevIL/lib/x86/Release/ILU.lib" )
#			pragma comment( lib, "DevIL/lib/x86/Release/ILUT.lib" )
#		else // RELEASE
#			pragma comment(lib, "MathGeoLib/libx86/Release/MathGeoLib.lib")
#			pragma comment(lib, "Glew/lib/Release/Win32/glew32.lib")
#			pragma comment( lib, "DevIL/lib/x86/Release/DevIL.lib" )
#			pragma comment( lib, "DevIL/lib/x86/Release/ILU.lib" )
#			pragma comment( lib, "DevIL/lib/x86/Release/ILUT.lib" )
#		endif // _DEBUG
#	endif // _WIN64
#endif // _MSC_VER

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	ReportMemoryLeaks();

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			DLOG("Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			DLOG("Application Init --------------");
			if (App->Init() == false)
			{
				DLOG("Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				DLOG("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				DLOG("Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			DLOG("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				DLOG("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	RELEASE( App);
	DLOG("Bye :)\n");
	return main_return;
}