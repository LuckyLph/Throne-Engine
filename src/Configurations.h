#pragma once

#define PLATFORM_WINDOWS
#define VK_NO_PROTOTYPES

#include <iostream>
#include <string>

#if defined PLATFORM_WINDOWS
#include <Windows.h>
#define LIBRARY_TYPE HMODULE
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined PLATFORM_LINUX
#define LIBRARY_TYPE void *
#endif

#define THRONE_API_ENTRY extern

#define ERROR_VALIDATION_LEVEL_ALL 0
#define ERROR_VALIDATION_LEVEL_NOTHING 1

#define ERROR_VALIDATION_LEVEL 0

#if defined _DEBUG
const bool enableValidationLayers = true;
#else 
const bool enableValidationLayers = false;
#endif

#if ERROR_VALIDATION_LEVEL == ERROR_VALIDATION_LEVEL_ALL

#define ERROR_VALIDATION_NULLPTR_CHECK
#define ERROR_VALIDATION_VKSUCCESS_CHECK

#define ERROR_MESSAGE_LEVEL_NO_MESSAGES 0
#define ERROR_MESSAGE_LEVEL_CONSOLE 1
#define ERROR_MESSAGE_LEVEL_EXCEPTION 2


#define ERROR_MESSAGE_LEVEL ERROR_MESSAGE_LEVEL_CONSOLE

#define ERROR_MESSAGE_NEEDED ERROR_MESSAGE_LEVEL != ERROR_MESSAGE_LEVEL_NO_MESSAGES

#if ERROR_MESSAGE_LEVEL == ERROR_MESSAGE_LEVEL_CONSOLE
#define ERROR_MESSAGE(message) std::cerr << (message) << "\n"
#elif ERROR_MESSAGE_LEVEL == ERROR_MESSAGE_LEVEL_EXCEPTION
#define ERROR_MESSAGE(message) throw std::runtime_error(message);
#else
#define ERROR_MESSAGE(message)
#endif // ERROR_MESSAGE_LEVEL check

#endif // ERROR_VALIDATION_LEVEL check