#pragma once
#include "stdafx.h"
#include "VulkanFunctions.h"

namespace ThroneEngine
{
    LIBRARY_TYPE vulkanLibrary;

    // Global level

    PFN_vkGetInstanceProcAddr funcptr_vkGetInstanceProcAddr;
    PFN_vkEnumerateInstanceExtensionProperties funcptr_vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumerateInstanceLayerProperties funcptr_vkEnumerateInstanceLayerProperties;
    PFN_vkCreateInstance funcptr_vkCreateInstance;

    #pragma region Instance level

    PFN_vkEnumeratePhysicalDevices funcptr_vkEnumeratePhysicalDevices;
    PFN_vkGetPhysicalDeviceProperties funcptr_vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceFeatures funcptr_vkGetPhysicalDeviceFeatures;
    PFN_vkCreateDevice funcptr_vkCreateDevice;
    PFN_vkGetDeviceProcAddr funcptr_vkGetDeviceProcAddr;
    PFN_vkEnumerateDeviceExtensionProperties funcptr_vkEnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties funcptr_vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkDestroyInstance funcptr_vkDestroyInstance;

    #pragma region Instance level from extension

    PFN_vkGetPhysicalDeviceSurfaceSupportKHR funcptr_vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR funcptr_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR funcptr_vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR funcptr_vkGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkDestroySurfaceKHR funcptr_vkDestroySurfaceKHR;

    #if defined VK_USE_PLATFORM_WIN32_KHR
    PFN_vkCreateWin32SurfaceKHR funcptr_vkCreateWin32SurfaceKHR;

    #elif defined VK_USE_PLATFORM_XCB_KHR
    PFN_vkCreateXcbSurfaceKHR funcptr_vkCreateXcbSurfaceKHR;

    #elif defined VK_USE_PLATFORM_XLIB_KHR
    PFN_vkCreateXlibSurfaceKHR funcptr_vkCreateXlibSurfaceKHR;

    #endif

    #pragma endregion Instance level from extension
    #pragma endregion Instance level

    #pragma region Device level

    PFN_vkGetDeviceQueue funcptr_vkGetDeviceQueue;
    PFN_vkDeviceWaitIdle funcptr_vkDeviceWaitIdle;
    PFN_vkDestroyDevice funcptr_vkDestroyDevice;
    PFN_vkCreateBuffer funcptr_vkCreateBuffer;
    PFN_vkGetBufferMemoryRequirements funcptr_vkGetBufferMemoryRequirements;
    PFN_vkCreateCommandPool funcptr_vkCreateCommandPool;
    PFN_vkCreateImageView funcptr_vkCreateImageView;
    PFN_vkDestroyImageView funcptr_vkDestroyImageView;
    PFN_vkAllocateCommandBuffers funcptr_vkAllocateCommandBuffers;
    PFN_vkCreateShaderModule funcptr_vkCreateShaderModule;
    PFN_vkDestroyShaderModule funcptr_vkDestroyShaderModule;

    #pragma region Device level from extension

    PFN_vkCreateSwapchainKHR funcptr_vkCreateSwapchainKHR;
    PFN_vkGetSwapchainImagesKHR funcptr_vkGetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR funcptr_vkAcquireNextImageKHR;
    PFN_vkQueuePresentKHR funcptr_vkQueuePresentKHR;
    PFN_vkDestroySwapchainKHR funcptr_vkDestroySwapchainKHR;

    #pragma endregion Device level from extension
    #pragma endregion Device level
}



