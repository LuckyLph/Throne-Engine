#pragma once
#include "stdafx.h"
#include "VulkanFunctions.h"

#include "R.h"

#include <vector>
#include <string>
#include <cassert>

namespace ThroneEngine
{
    class Window
    {
        public:
        Window(uint32_t sizeX, uint32_t sizeY, std::string name);
        ~Window();

        void close();
        bool update();

        bool initialize(VkInstance instance);
        bool initOSWindow();
        void deInitOSWindow();
        void updateOSWindow();
        void initOSSurface(VkInstance instance);

        bool initSurface(VkInstance instance, VkPhysicalDevice physicalDevice);
        void deInitSurface(VkInstance instance);

        bool initSwapChain(VkDevice device, VkPhysicalDevice physicalDevice);
        void deInitSwapChain(VkDevice device);

        bool initSwapChainImages(VkDevice device);
        void deInitSwapChainImages(VkDevice device);

        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkSwapchainKHR swapchain = VK_NULL_HANDLE;

        uint32_t surfaceSizeX = R::C::DEFAULT_WINDOW_WIDTH;
        uint32_t surfaceSizeY = R::C::DEFAULT_WINDOW_HEIGTH;
        std::string windowName = R::S::StandardOutput::sdo_windowName;
        uint32_t swapchainImageCount = R::C::DEFAULT_SWAPCHAIN_IMAGE_COUNT;

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VkSurfaceFormatKHR surfaceFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkImage> swapchainImages;
        std::vector<VkImageView> swapchainImageViews;

        bool windowShouldRun = true;

        #if defined VK_USE_PLATFORM_WIN32_KHR
        HINSTANCE win32Instance = NULL;
        HWND win32Window = NULL;
        std::string win32ClassName;
        static uint64_t win32ClassIdCounter;
        #endif

    };
}

