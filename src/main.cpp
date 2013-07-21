#include <allegro.h> // END_OF_MAIN
#include "basictypes.hpp"
#include "system.hpp"

using namespace kt;

int main() {
	System.Init();
	System.Run();
	System.Shutdown();
	return 0;
}
END_OF_MAIN();
