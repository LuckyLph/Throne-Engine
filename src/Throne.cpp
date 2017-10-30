#pragma once
#include "stdafx.h"
#include "Throne.h"

using namespace R::S::ErrorMessage;
using namespace R::S::Path;
using namespace R::S::StandardOutput;
using namespace R::C;

namespace ThroneEngine
{
    Throne::Throne()
    {
        window = new Window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGTH, sdo_windowName);
        if (initializeThroneFramework() == false)
        {
            ERROR_MESSAGE(err_throneInitialization);
        }
    }

    Throne::~Throne()
    {

    }

    void Throne::run()
    {
        while (window->update())
        {

        }
    }
}

