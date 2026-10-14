#include "OrionEngine.h"

#include "EngineEntry.h"

int main(int argc, char** argv)
{
	Orion::Engine::Initialize(argc, argv);
	Orion::Engine::Shutdown();
	return ORION_EXIT_SUCCESS;
}
