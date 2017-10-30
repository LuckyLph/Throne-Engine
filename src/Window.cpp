#pragma once
#include "stdafx.h"
#include "Window.h"

using namespace R::S::ErrorMessage;
using namespace R::S::Path;
using namespace R::S::StandardOutput;

namespace ThroneEngine
{
    Window::Window(uint32_t sizeX, uint32_t sizeY, std::string name)
    {
        surfaceSizeX = sizeX;
        surfaceSizeY = sizeY;
        windowName = name;
    }

    Window::~Window()
    {
        deInitOSWindow();
    }

    void Window::close()
    {
        windowShouldRun = false;
    }

    bool Window::update()
    {
        updateOSWindow();
        return windowShouldRun;
    }

    bool Window::initialize(VkInstance instance)
    {
        if (initOSWindow() == false)
            return false;
        initOSSurface(instance);
        return false;
    }

    bool Window::initSurface(VkInstance instance, VkPhysicalDevice physicalDevice)
    {
        VkBool32 WSISupported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface, &WSISupported);
        if (!WSISupported)
        {
            return false;
        }
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
        if (surfaceCapabilities.currentExtent.width < UINT32_MAX)
        {
            swapChainExtent.width = surfaceCapabilities.currentExtent.width;
            swapChainExtent.height = surfaceCapabilities.currentExtent.height;
        }
        {
            uint32_t formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
            if (formatCount == 0)
            {
                return false;
            }
            std::vector<VkSurfaceFormatKHR> formats(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
            if (formats[0].format == VK_FORMAT_UNDEFINED)
            {
                surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
                surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
            }
            else
            {
                surfaceFormat = formats[0];
            }
        }
        return true;
    }

    void Window::deInitSurface(VkInstance instance)
    {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }

    bool Window::initSwapChain(VkDevice device, VkPhysicalDevice physicalDevice)
    {
        if (swapchainImageCount > surfaceCapabilities.maxImageCount && surfaceCapabilities.maxImageCount > 0)
            swapchainImageCount = surfaceCapabilities.maxImageCount;
        if (swapchainImageCount < surfaceCapabilities.minImageCount + 1)
            swapchainImageCount = surfaceCapabilities.minImageCount + 1;
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        {
            uint32_t presentModeCount = 0;

            VkResult error = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
            #if defined(ERROR_VALIDATION_VKSUCCESS_CHECK)
            if (error != VK_SUCCESS)
            {
                #if defined(ERROR_MESSAGE_NEEDED)
                ERROR_MESSAGE(err_physicalSurfacePresentMode);
                #endif
                return false;
            }
            #endif

            std::vector<VkPresentModeKHR> presentModeList(presentModeCount);

            error = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModeList.data());
            #if defined(ERROR_VALIDATION_VKSUCCESS_CHECK)
            if (error != VK_SUCCESS)
            {
                #if defined(ERROR_MESSAGE_NEEDED)
                ERROR_MESSAGE(err_physicalSurfacePresentMode);
                #endif
                return false;
            }
            #endif

            for (VkPresentModeKHR m : presentModeList)
            {
                if (m == VK_PRESENT_MODE_MAILBOX_KHR)
                    presentMode = m;
            }
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = surface;
        swapchainCreateInfo.minImageCount = swapchainImageCount;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent.width = swapChainExtent.width;
        swapchainCreateInfo.imageExtent.height = swapChainExtent.height;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;
        swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult error = vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain);
        #if defined(ERROR_VALIDATION_VKSUCCESS_CHECK)
        if (error != VK_SUCCESS)
        {
            #if defined(ERROR_MESSAGE_NEEDED)
            ERROR_MESSAGE(err_swapChain);
            #endif
            return false;
        }
        #endif

        error = vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, nullptr);
        #if defined(ERROR_VALIDATION_VKSUCCESS_CHECK)
        if (error != VK_SUCCESS)
        {
            #if defined(ERROR_MESSAGE_NEEDED)
            ERROR_MESSAGE(err_swapChainImages);
            #endif
            return false;
        }
        #endif
    }

    void Window::deInitSwapChain(VkDevice device)
    {
        vkDestroySwapchainKHR(device, swapchain, nullptr);
    }

    bool Window::initSwapChainImages(VkDevice device)
    {
        swapchainImages.resize(swapchainImageCount);
        swapchainImageViews.resize(swapchainImageCount);
        VkResult error = vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages.data());
        #if defined(ERROR_VALIDATION_VKSUCCESS_CHECK)
        if (error != VK_SUCCESS)
        {
            #if defined(ERROR_MESSAGE_NEEDED)
            ERROR_MESSAGE(err_swapChainImages);
            #endif
            return false;
        }
        #endif

        for (uint32_t i = 0; i < swapchainImageCount; i++)
        {
            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = swapchainImages[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = surfaceFormat.format;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            error = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &swapchainImageViews[i]);
            #if defined(ERROR_VALIDATION_VKSUCCESS_CHECK)
            if (error != VK_SUCCESS)
            {
                #if defined(ERROR_MESSAGE_NEEDED)
                ERROR_MESSAGE(err_imageViewCreation);
                #endif
                return false;
            }
            #endif
        }
    }

    void Window::deInitSwapChainImages(VkDevice device)
    {
        for (VkImageView imageView : swapchainImageViews)
        {
            vkDestroyImageView(device, imageView, nullptr);
        }
    }
}
