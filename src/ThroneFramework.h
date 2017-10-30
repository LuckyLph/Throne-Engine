#pragma once
#include "stdafx.h"

#ifndef ThroneFramework_h
#define ThroneFramework_h

#include "Window.h"
#include "R.h";

#include <string>
#include <vector>
#include <fstream>
#include <memory>

namespace ThroneEngine
{

    struct QueueInfo
    {
        uint32_t familyIndex;
        std::vector<float> priorities;
    };

    THRONE_API_ENTRY bool initializeThroneFramework();
    THRONE_API_ENTRY void destroyThroneFramework();

    THRONE_API_ENTRY VkInstance instance;
    THRONE_API_ENTRY VkPhysicalDevice physicalDevice;
    THRONE_API_ENTRY VkDevice logicalDevice;
    THRONE_API_ENTRY Window* window;

    THRONE_API_ENTRY VkQueue selectedQueue;
    THRONE_API_ENTRY VkCommandPool commandPool;

    THRONE_API_ENTRY VkPresentModeKHR desiredPresentMode;
    THRONE_API_ENTRY VkPresentModeKHR presentMode;

    THRONE_API_ENTRY bool createVulkanInstance(std::vector<const char*>& requiredExtensions);
    THRONE_API_ENTRY bool createGraphicsPipeline();
    THRONE_API_ENTRY VkShaderModule createShaderModule(const std::vector<char>& code);
    THRONE_API_ENTRY bool checkAvailableExtensions(const bool enumerateExtensions, std::vector<VkExtensionProperties>& availableExtensions);
    THRONE_API_ENTRY bool verifyRequiredExtensions(std::vector<VkExtensionProperties>& availableExtensions, std::vector<const char*>& requiredExtensions, std::vector<const char*>& unsupportedExtensions);
    THRONE_API_ENTRY void enumerateExtensions(std::vector<VkExtensionProperties>& availableExtensions);
    THRONE_API_ENTRY bool enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& physicalDevices);
    THRONE_API_ENTRY VkPhysicalDevice findSuitablePhysicalDevice(std::vector<VkPhysicalDevice>& physicalDevices, std::vector<const char*>& requiredDeviceExtensions, VkPhysicalDeviceFeatures& requiredFeatures, std::vector<QueueInfo>& queueInfos, VkSurfaceKHR surface);
    THRONE_API_ENTRY bool enumerateAvailableExtensionsOnDevice(VkPhysicalDevice& device, std::vector<VkExtensionProperties>& availableExtensions);
    THRONE_API_ENTRY bool requiredFeaturesAreSupported(VkPhysicalDeviceFeatures& requiredFeatures, VkPhysicalDeviceFeatures& availableFeatures);
    THRONE_API_ENTRY bool enumerateDeviceQueueFamilies(VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queueFamilies);
    THRONE_API_ENTRY int findSuitableQueueFamily(std::vector<VkQueueFamilyProperties>& queueFamilies, VkQueueFlags desiredCapabilities);
    THRONE_API_ENTRY bool createLogicalDevice(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, std::vector<char const*>& requiredDeviceExtensions, std::vector<QueueInfo>& queueInfos, VkPhysicalDeviceFeatures& requiredFeatures);
    THRONE_API_ENTRY int findSuitableQueueFamilyForSwapchainPresentation(VkPhysicalDevice physicalDevice, std::vector<VkQueueFamilyProperties>& queueFamilies, VkSurfaceKHR surface);
    THRONE_API_ENTRY bool getAvailablePresentationModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& availablePresentModes);
    THRONE_API_ENTRY bool verifyDesiredPresentationModeIsAvailable(std::vector<VkPresentModeKHR>& availablePresentModes, VkPresentModeKHR desiredPresentMode);
    THRONE_API_ENTRY bool getSurfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& surfaceCapabilities);
    THRONE_API_ENTRY void setFeaturesToFalse(VkPhysicalDeviceFeatures& features);
    THRONE_API_ENTRY bool createCommandPool(VkDevice logicalDevice, QueueInfo& queueInfo, VkCommandPool commandPool, VkFlags parameters);
    THRONE_API_ENTRY bool createCommandBuffers(VkDevice logicalDevice, VkCommandPool commandPool, std::vector<VkCommandBuffer>& commandBuffers, const int numberOfBufferToCreate);
    THRONE_API_ENTRY bool submitRequestToBuffer();
    THRONE_API_ENTRY std::vector<char> readFile(const std::string& filename);

    #pragma region Temporaire
    #if defined VK_USE_PLATFORM_WIN32_KHR
    THRONE_API_ENTRY void addWin32SurfaceExtensions(std::vector<const char*>& requiredExtensions);
    #endif

    THRONE_API_ENTRY void addSwapchainExtension(std::vector<const char*>& requiredExtensions);

    #pragma endregion

}



#endif // !ThroneFramework_h
