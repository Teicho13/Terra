#include <Core/Application.h>

#include "World.h"

int main(int argc, char* argv[])
{
    Terra::Application Application;

    Application.PushScene<World>();
    Application.Run();
    
	return 0;
}
