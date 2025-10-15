#include <Core/Application.h>
#include "Scenes/TestScene.h"

int main(int argc, char* argv[])
{
    Terra::Application Application;

    //Create testing scene
    Application.PushScene<TestScene>();
    Application.Run();
    
	return 0;
}
