#pragma once

#ifndef VULKAN_FUNCTIONS
#define VULKAN_FUNCTIONS

#include "vulkan.h"
#include "R.h"

#include <string>
#include <vector>

namespace ThroneEngine
{
    template<class F, class L>
    THRONE_API_ENTRY inline bool loadFunctionFromLibrary(F& func, const L library, const std::string funcName);
    template<class F, class L>
    THRONE_API_ENTRY inline bool loadFunctionFromLibrary(F& func, const L library, const char* funcName);

    template<class F, class I>
    THRONE_API_ENTRY inline bool loadVulkanInstanceFunction(const PFN_vkGetInstanceProcAddr funcLoader, F& func, const I instance, const char* funcName);

    template<class F, class D>
    THRONE_API_ENTRY inline bool loadVulkanDeviceFunction(const PFN_vkGetDeviceProcAddr funcLoader, F& func, const D device, const char* funcName);



    THRONE_API_ENTRY LIBRARY_TYPE vulkanLibrary;

    THRONE_API_ENTRY bool isExtensionSupported(std::vector<VkExtensionProperties>& availableExtensions, const char* requiredExtension);
    THRONE_API_ENTRY bool isExtensionSupported(std::vector<const char*>& availableExtensions, const char* requiredExtension);

    THRONE_API_ENTRY bool loadVulkanLibrary();
    THRONE_API_ENTRY void releaseVulkanLibrary();
    THRONE_API_ENTRY bool loadPFNGetInstanceProcAddr();

    THRONE_API_ENTRY bool initializeVulkanFunctions();
    THRONE_API_ENTRY bool initializeInstanceLevelVulkanFunctions(const VkInstance& instance, std::vector<const char*>& enabledExtensions);
    THRONE_API_ENTRY bool initializeDeviceLevelVulkanFunctions(const VkDevice& logicalDevice, std::vector<const char*>& enabledExtensions);


    // Global level

    THRONE_API_ENTRY PFN_vkGetInstanceProcAddr funcptr_vkGetInstanceProcAddr;
    THRONE_API_ENTRY PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance instance, const char* pName);

    THRONE_API_ENTRY PFN_vkEnumerateInstanceExtensionProperties funcptr_vkEnumerateInstanceExtensionProperties;
    THRONE_API_ENTRY VkResult vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);

    THRONE_API_ENTRY PFN_vkEnumerateInstanceLayerProperties funcptr_vkEnumerateInstanceLayerProperties;
    THRONE_API_ENTRY VkResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties);

    THRONE_API_ENTRY PFN_vkCreateInstance funcptr_vkCreateInstance;
    THRONE_API_ENTRY VkResult vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

    #pragma region Instance level

    THRONE_API_ENTRY PFN_vkEnumeratePhysicalDevices funcptr_vkEnumeratePhysicalDevices;
    THRONE_API_ENTRY VkResult vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);

    THRONE_API_ENTRY PFN_vkGetPhysicalDeviceProperties funcptr_vkGetPhysicalDeviceProperties;
    THRONE_API_ENTRY void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);

    THRONE_API_ENTRY PFN_vkGetPhysicalDeviceFeatures funcptr_vkGetPhysicalDeviceFeatures;
    THRONE_API_ENTRY void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);

    THRONE_API_ENTRY PFN_vkCreateDevice funcptr_vkCreateDevice;
    THRONE_API_ENTRY VkResult vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

    THRONE_API_ENTRY PFN_vkGetDeviceProcAddr funcptr_vkGetDeviceProcAddr;
    THRONE_API_ENTRY PFN_vkVoidFunction vkGetDeviceProcAddr(VkDevice device, const char* pName);

    THRONE_API_ENTRY PFN_vkEnumerateDeviceExtensionProperties funcptr_vkEnumerateDeviceExtensionProperties;
    THRONE_API_ENTRY VkResult vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);


    THRONE_API_ENTRY PFN_vkGetPhysicalDeviceQueueFamilyProperties funcptr_vkGetPhysicalDeviceQueueFamilyProperties;
    THRONE_API_ENTRY void vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);

    THRONE_API_ENTRY PFN_vkDestroyInstance funcptr_vkDestroyInstance;
    THRONE_API_ENTRY void vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator);

    #pragma region Instance level from extension

    THRONE_API_ENTRY PFN_vkGetPhysicalDeviceSurfaceSupportKHR funcptr_vkGetPhysicalDeviceSurfaceSupportKHR;
    THRONE_API_ENTRY VkResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);

    THRONE_API_ENTRY PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR funcptr_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    THRONE_API_ENTRY VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);

    THRONE_API_ENTRY PFN_vkGetPhysicalDeviceSurfaceFormatsKHR funcptr_vkGetPhysicalDeviceSurfaceFormatsKHR;
    THRONE_API_ENTRY VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);

    THRONE_API_ENTRY PFN_vkGetPhysicalDeviceSurfacePresentModesKHR funcptr_vkGetPhysicalDeviceSurfacePresentModesKHR;
    THRONE_API_ENTRY VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);

    THRONE_API_ENTRY PFN_vkDestroySurfaceKHR funcptr_vkDestroySurfaceKHR;
    THRONE_API_ENTRY void vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);

    #if defined VK_USE_PLATFORM_WIN32_KHR
    THRONE_API_ENTRY PFN_vkCreateWin32SurfaceKHR funcptr_vkCreateWin32SurfaceKHR;
    THRONE_API_ENTRY VkResult vkCreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

    #elif defined VK_USE_PLATFORM_XCB_KHR
    THRONE_API_ENTRY PFN_vkCreateXcbSurfaceKHR funcptr_vkCreateXcbSurfaceKHR;
    THRONE_API_ENTRY VkResult PFN_vkCreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

    #elif defined VK_USE_PLATFORM_XLIB_KHR
    THRONE_API_ENTRY PFN_vkCreateXlibSurfaceKHR funcptr_vkCreateXlibSurfaceKHR;
    THRONE_API_ENTRY VkResult vkCreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

    #endif

    #pragma endregion Instance level from extension

    #pragma endregion Instance level

    // Device level

    THRONE_API_ENTRY PFN_vkGetDeviceQueue funcptr_vkGetDeviceQueue;
    THRONE_API_ENTRY void vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);

    THRONE_API_ENTRY PFN_vkDeviceWaitIdle funcptr_vkDeviceWaitIdle;
    THRONE_API_ENTRY VkResult vkDeviceWaitIdle(VkDevice device);

    THRONE_API_ENTRY PFN_vkDestroyDevice funcptr_vkDestroyDevice;
    THRONE_API_ENTRY void vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator);

    THRONE_API_ENTRY PFN_vkCreateBuffer funcptr_vkCreateBuffer;
    THRONE_API_ENTRY VkResult vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);

    THRONE_API_ENTRY PFN_vkGetBufferMemoryRequirements funcptr_vkGetBufferMemoryRequirements;
    THRONE_API_ENTRY void vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);

    THRONE_API_ENTRY PFN_vkCreateCommandPool funcptr_vkCreateCommandPool;
    THRONE_API_ENTRY VkResult vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);

    THRONE_API_ENTRY PFN_vkAllocateCommandBuffers funcptr_vkAllocateCommandBuffers;
    THRONE_API_ENTRY VkResult vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);

    THRONE_API_ENTRY PFN_vkCreateImageView funcptr_vkCreateImageView;
    THRONE_API_ENTRY VkResult vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);

    THRONE_API_ENTRY PFN_vkDestroyImageView funcptr_vkDestroyImageView;
    THRONE_API_ENTRY void vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator);

    THRONE_API_ENTRY PFN_vkCreateShaderModule funcptr_vkCreateShaderModule;
    THRONE_API_ENTRY VkResult vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);

    THRONE_API_ENTRY PFN_vkDestroyShaderModule funcptr_vkDestroyShaderModule;
    THRONE_API_ENTRY void vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);

    // Device level from extension

    THRONE_API_ENTRY PFN_vkCreateSwapchainKHR funcptr_vkCreateSwapchainKHR;
    THRONE_API_ENTRY VkResult vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);

    THRONE_API_ENTRY PFN_vkGetSwapchainImagesKHR funcptr_vkGetSwapchainImagesKHR;
    THRONE_API_ENTRY VkResult vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);

    THRONE_API_ENTRY PFN_vkAcquireNextImageKHR funcptr_vkAcquireNextImageKHR;
    THRONE_API_ENTRY VkResult vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);

    THRONE_API_ENTRY PFN_vkQueuePresentKHR funcptr_vkQueuePresentKHR;
    THRONE_API_ENTRY VkResult vkQueuePresentKHR(VkQueue graphicsQueue, const VkPresentInfoKHR* pPresentInfo);

    THRONE_API_ENTRY PFN_vkDestroySwapchainKHR funcptr_vkDestroySwapchainKHR;
    THRONE_API_ENTRY void vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);

    //


}

#endif // !VULKAN_FUNCTIONS