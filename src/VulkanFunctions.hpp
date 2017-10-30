#pragma once
#include "VulkanFunctions.h"

#include <iostream>

using namespace R::S::ErrorMessage;
using namespace R::S::Path;
using namespace R::S::StandardOutput;

using std::cout;

namespace ThroneEngine
{

    template<class F, class L>
    inline bool loadFunctionFromLibrary(F& func, const L library, const std::string funcName)
    {
        return loadFunctionFromLibrary(func, library, funcName.c_str());
    }

    template<class F, class L>
    inline bool loadFunctionFromLibrary(F& func, const L library, const char* funcName)
    {
        #if defined PLATFORM_WINDOWS
        func = (F)GetProcAddress(library, funcName);
        #elif defined PLATFORM_LINUX
        func = (T)dlsym(library, funcName);
        #endif

        #if defined ERROR_VALIDATION_NULLPTR_CHECK
        if (func == nullptr)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_functionLoad + std::string(funcName));
            #endif
            return false;
        }
        #endif
        return true;
    }

    template<class F, class I>
    inline bool loadVulkanInstanceFunction(const PFN_vkGetInstanceProcAddr funcLoader, F& func, const I instance, const char* funcName)
    {
        func = (F)funcLoader(instance, funcName);
        #if defined ERROR_VALIDATION_NULLPTR_CHECK
        if (func == nullptr)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_vulkanInstanceFunctionLoad + std::string(funcName));
            #endif
            return false;
        }
        #endif
        return true;
    }

    template<class F, class D>
    inline bool loadVulkanDeviceFunction(const PFN_vkGetDeviceProcAddr funcLoader, F& func, const D device, const char* funcName)
    {
        func = (F)funcLoader(device, funcName);
        #if defined ERROR_VALIDATION_NULLPTR_CHECK
        if (func == nullptr)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_vulkanDeviceFunctionLoad + std::string(funcName));
            #endif
            return false;
        }
        #endif
        return true;
    }
}