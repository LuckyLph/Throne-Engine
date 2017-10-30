#pragma once
#include "stdafx.h"
#include "VulkanFunctions.hpp"

using std::cout;

namespace ThroneEngine
{
    bool loadVulkanLibrary()
    {
        #if defined PLATFORM_WINDOWS
        vulkanLibrary = LoadLibrary(_T("vulkan-1.dll"));
        #elif defined PLATFORM_LINUX
        vulkanLibrary = dlopen("libvulkan.so.1", RTLD_NOW);
        #endif
        #if defined ERROR_VALIDATION_NULLPTR_CHECK
        if (vulkanLibrary == nullptr)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_vulkanLibraryConnect);
            #endif
            return false;
        }
        #endif
        return true;
    }

    inline void releaseVulkanLibrary()
    {
        #if defined PLATFORM_WINDOWS
        FreeLibrary(vulkanLibrary);
        #elif defined PLATFORM_LINUX
        dlclose(vulkanLibrary);
        #endif
        vulkanLibrary = nullptr;
    }

    inline bool isExtensionSupported(std::vector<VkExtensionProperties>& availableExtensions, const char* requiredExtension)
    {
        for (VkExtensionProperties& availableExtension : availableExtensions)
        {
            if (strcmp(requiredExtension, availableExtension.extensionName) == 0)
            {
                return true;
            }
        }
        return false;
    }

    inline bool isExtensionSupported(std::vector<const char*>& availableExtensions, const char* requiredExtension)
    {
        for (const char * & availableExtension : availableExtensions)
        {
            if (strcmp(requiredExtension, availableExtension) == 0)
            {
                return true;
            }
        }
        return false;
    }


    inline bool initializeVulkanFunctions()
    {
        if (loadVulkanLibrary() == false)
        {
            return false;
        }
        if (loadPFNGetInstanceProcAddr() == false)
        {
            return false;
        }

        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkEnumerateInstanceExtensionProperties, nullptr, "vkEnumerateInstanceExtensionProperties");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkEnumerateInstanceLayerProperties, nullptr, "vkEnumerateInstanceLayerProperties");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkCreateInstance, nullptr, "vkCreateInstance");

        return true;
    }

    inline bool initializeInstanceLevelVulkanFunctions(const VkInstance& instance, std::vector<const char*>& enabledExtensions)
    {
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkEnumeratePhysicalDevices, instance, "vkEnumeratePhysicalDevices");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkEnumerateDeviceExtensionProperties, instance, "vkEnumerateDeviceExtensionProperties");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkGetPhysicalDeviceFeatures, instance, "vkGetPhysicalDeviceFeatures");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkGetPhysicalDeviceProperties, instance, "vkGetPhysicalDeviceProperties");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkGetPhysicalDeviceQueueFamilyProperties, instance, "vkGetPhysicalDeviceQueueFamilyProperties");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkCreateDevice, instance, "vkCreateDevice");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkGetDeviceProcAddr, instance, "vkGetDeviceProcAddr");
        loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkDestroyInstance, instance, "vkDestroyInstance");

        if (isExtensionSupported(enabledExtensions, VK_KHR_SURFACE_EXTENSION_NAME))
        {
            loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkGetPhysicalDeviceSurfaceSupportKHR, instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
            loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkGetPhysicalDeviceSurfaceCapabilitiesKHR, instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
            loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkGetPhysicalDeviceSurfaceFormatsKHR, instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
            loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkGetPhysicalDeviceSurfacePresentModesKHR, instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
            loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkDestroySurfaceKHR, instance, "vkDestroySurfaceKHR");
        }

        #if defined VK_USE_PLATFORM_WIN32_KHR
        if (isExtensionSupported(enabledExtensions, VK_KHR_WIN32_SURFACE_EXTENSION_NAME))
        {
            loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkCreateWin32SurfaceKHR, instance, "vkCreateWin32SurfaceKHR");
        }
        #elif defined VK_USE_PLATFORM_XCB_KHR
        if (IsExtensionSupported(enabledExtensions, VK_KHR_XCB_SURFACE_EXTENSION_NAME))
        {
            loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkCreateXcbSurfaceKHR, instance, "vkCreateXcbSurfaceKHR");
        }
        #elif defined VK_USE_PLATFORM_XLIB_KHR
        if (IsExtensionSupported(enabledExtensions, VK_KHR_XLIB_SURFACE_EXTENSION_NAME))
        {
            loadVulkanInstanceFunction(funcptr_vkGetInstanceProcAddr, funcptr_vkCreateXlibSurfaceKHR, instance, "vkCreateXlibSurfaceKHR");
        }
        #endif


        return true;
    }

    inline bool initializeDeviceLevelVulkanFunctions(const VkDevice& logicalDevice, std::vector<const char*>& enabledExtensions)
    {
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkGetDeviceQueue, logicalDevice, "vkGetDeviceQueue");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkDeviceWaitIdle, logicalDevice, "vkDeviceWaitIdle");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkDestroyDevice, logicalDevice, "vkDestroyDevice");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkCreateBuffer, logicalDevice, "vkCreateBuffer");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkGetBufferMemoryRequirements, logicalDevice, "vkGetBufferMemoryRequirements");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkCreateCommandPool, logicalDevice, "vkCreateCommandPool");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkAllocateCommandBuffers, logicalDevice, "vkAllocateCommandBuffers");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkCreateImageView, logicalDevice, "vkCreateImageView");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkDestroyImageView, logicalDevice, "vkDestroyImageView");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkCreateShaderModule, logicalDevice, "vkCreateShaderModule");
        loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkDestroyShaderModule, logicalDevice, "vkDestroyShaderModule");

        if (isExtensionSupported(enabledExtensions, VK_KHR_SWAPCHAIN_EXTENSION_NAME))
        {
            loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkCreateSwapchainKHR, logicalDevice, "vkCreateSwapchainKHR");
            loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkGetSwapchainImagesKHR, logicalDevice, "vkGetSwapchainImagesKHR");
            loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkAcquireNextImageKHR, logicalDevice, "vkAcquireNextImageKHR");
            loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkQueuePresentKHR, logicalDevice, "vkQueuePresentKHR");
            loadVulkanDeviceFunction(funcptr_vkGetDeviceProcAddr, funcptr_vkDestroySwapchainKHR, logicalDevice, "vkDestroySwapchainKHR");
        }

        return true;
    }


    VkResult vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
    {
        return funcptr_vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
    }

    VkResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
    {
        return funcptr_vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
    }

    VkResult vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
    {
        return funcptr_vkCreateInstance(pCreateInfo, pAllocator, pInstance);
    }

    inline VkResult vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
    {
        return funcptr_vkEnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    }

    inline void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
    {
        funcptr_vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
    }

    inline void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
    {
        funcptr_vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
    }

    inline VkResult vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
    {
        return funcptr_vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    }

    inline PFN_vkVoidFunction vkGetDeviceProcAddr(VkDevice device, const char* pName)
    {
        return funcptr_vkGetDeviceProcAddr(device, pName);
    }

    inline VkResult vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
    {
        return funcptr_vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
    }

    inline void vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
    {
        funcptr_vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    }

    inline VkResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
    {
        return funcptr_vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
    }

    inline VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
    {
        return funcptr_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
    }

    inline VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
    {
        return funcptr_vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    }

    inline VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
    {
        return funcptr_vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
    }

    void vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
    {
        funcptr_vkDestroySurfaceKHR(instance, surface, pAllocator);
    }

    inline void vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
    {
        funcptr_vkDestroyInstance(instance, pAllocator);
    }



    PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance instance, const char* pName)
    {
        return funcptr_vkGetInstanceProcAddr(instance, pName);
    }

    bool loadPFNGetInstanceProcAddr()
    {
        return loadFunctionFromLibrary(funcptr_vkGetInstanceProcAddr, vulkanLibrary, "vkGetInstanceProcAddr");
    }

    #if defined VK_USE_PLATFORM_WIN32_KHR
    inline VkResult vkCreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
    {
        return funcptr_vkCreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    }

    #elif defined VK_USE_PLATFORM_XCB_KHR
    inline VkResult PFN_vkCreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
    {
        return funcptr_vkCreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    }

    #elif defined VK_USE_PLATFORM_XLIB_KHR
    inline VkResult vkCreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
    {
        return funcptr_vkCreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    }
    #endif


    inline void vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
    {
        funcptr_vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
    }

    inline VkResult vkDeviceWaitIdle(VkDevice device)
    {
        return funcptr_vkDeviceWaitIdle(device);
    }

    inline void vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
    {
        funcptr_vkDestroyDevice(device, pAllocator);
    }

    inline VkResult vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
    {
        return funcptr_vkCreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
    }

    inline void vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
    {
        funcptr_vkGetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
    }

    inline VkResult vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
    {
        return funcptr_vkCreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
    }

    inline VkResult vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
    {
        return funcptr_vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
    }

    inline VkResult vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
    {
        return funcptr_vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
    }

    inline VkResult vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
    {
        return funcptr_vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
    }

    inline VkResult vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
    {
        return funcptr_vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
    }

    inline VkResult vkQueuePresentKHR(VkQueue graphicsQueue, const VkPresentInfoKHR* pPresentInfo)
    {
        return funcptr_vkQueuePresentKHR(graphicsQueue, pPresentInfo);
    }

    inline void vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
    {
        funcptr_vkDestroySwapchainKHR(device, swapchain, pAllocator);
    }

    inline VkResult vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
    {
        return funcptr_vkCreateImageView(device, pCreateInfo, pAllocator, pView);
    }

    inline void vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
    {
        funcptr_vkDestroyImageView(device, imageView, pAllocator);
    }

    inline VkResult vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
    {
        return funcptr_vkCreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
    }

    inline void vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
    {
        funcptr_vkDestroyShaderModule(device, shaderModule, pAllocator);
    }
}
