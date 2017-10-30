#pragma once
#include "stdafx.h"

#include "Throne.h"
#include "R.h"

#include <iostream>

int main()
{
    ThroneEngine::Throne throne;

    try
    {
        throne.run();
    }
    catch (const std::exception& e)
    {
        #if defined ERROR_MESSAGE_NEEDED
        ERROR_MESSAGE(strcat(const_cast<char*>(R::S::ErrorMessage::err_unManaged.data()), e.what()));
        #endif
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

