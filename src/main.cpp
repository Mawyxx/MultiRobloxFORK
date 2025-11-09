#include "gui.hpp"

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE /*prevInstance*/, LPWSTR /*cmdLine*/, int showCommand)
{
    return RunGui(instance, showCommand);
}

