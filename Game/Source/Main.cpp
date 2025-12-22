#include <Core/Application.h>

#include "Scenes/PerlinVisualizerScene.h"
#include "Scenes/TestScene.h"

int main(int argc, char* argv[])
{
    Terra::Application Application;

    //Create testing scene
    Application.PushScene<TestScene>();
    //Application.PushScene<PerlinVisualizerScene>();
    Application.Run();
    
	return 0;
}
