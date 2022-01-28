#include "CollisionMapCollection.h"
#include "SampleMonsters.h"
#include "Window.h"

#include <iostream>

Window g_main_window;

int main()
{
	// Moved to Window class
   //CollisionMapCollection collision_map_collection;

   //SampleMonsters sample_monsters;
   //sample_monsters.OutputInitialLocationInfo();

   Window::EXIT_REASON result = g_main_window.Begin();
   std::cout << "Window has exited with code 0x0" << static_cast<int>(result) << std::endl;

   system("pause");
}
