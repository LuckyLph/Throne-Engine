#pragma once
#include "stdafx.h"
#include "R.h"

namespace R
{
    namespace S
    {
        namespace ErrorMessage
        {
            string err_unManaged = "Unmanaged error detected in main!";
            string err_unSupportedOperation = "Unsuported operation at ";
            string err_throneInitialization = "Could not initialize the Throne framework!";
            string err_VulkanInstance = "Could not create the Vulkan instance!";
            string err_extensionNumber = "Could not get the number of instance extensions!";
            string err_extensionEnumeration = "Could not enumerate instance extensions!";
            string err_unSupportedExtension = "Unsupported extension is required : ";
            string err_physicalDevicesNumber = "Could not get the number of physical devices!";
            string err_physicalDevicesEnumerate = "Could not enumerate physical devices!";
            string err_physicalDevicesSuitable = "Could not find a suitable physical device!";
            string err_deviceExtensionNumber = "Could not get the number of device extension properties!";
            string err_physicalDevice = "Could not get the physical devices!";
            string err_QueueFamilyNumber = "Could not get the number of queue families!";
            string err_QueueFamily = "Could not get the queue families!";
            string err_logicalDeviceCreation = "Could not create the logical device!";
            string err_physicalDeviceSupportForQueue = "Could not get the physical device surface support of a specific queue!";
            string err_queueFamilySupportForPresentation = "Could not find a queue family that supports presentation!";
            string err_deviceSurfaceNumber = "Could not get the number of device surface presentation modes!";
            string err_availablePresentationModes = "Could not get the available presentation modes!";
            string err_physicalDeviceCapabality = "Could not get the physical device surface capabilities!";
            string err_commandPoolCreation = "Could not create the command pool!";
            string err_commandBuffer = "Could not allocate the command buffers!";
            string err_vulkanLibrary = "Could not connect with a Vulkan Runtime Library!";
            string err_vulkanLibraryConnect = "Could not connect with a Vulkan Runtime Library!";
            string err_physicalSurfacePresentMode = "Could not get physical surface present modes!";
            string err_swapChainImages = "Could not get swapchain images!";
            string err_swapChain = "Could not create swapchain!";
            string err_imageViewCreation = "Could not create image view!";
            string err_fileOpen = "Could not open file!";
            string err_functionLoad = "Could not load function : ";
            string err_vulkanInstanceFunctionLoad = "Could not load vulkan instance function : ";
            string err_vulkanDeviceFunctionLoad = "Could not load vulkan device function : ";
            string err_shaderModuleCreation = "Could not create shader module!";
        }
        namespace StandardOutput
        {
            string sdo_windowName = "Throne Engine";
            string sdo_extensionNumber = "Number of extensions : ";
            string sdo_extension = " Extension : ";
            string sdo_shaderMethodToInvoke = "main";
        }
        namespace Path
        {
            string path_triangleShaderVert = "../shaders/vert.spv";
            string path_triangleShaderFrag = "../shaders/frag.spv";
        }
    }

    namespace C
    {
        uint32_t DEFAULT_WINDOW_WIDTH = 1280;
        uint32_t DEFAULT_WINDOW_HEIGTH = 720;
        uint32_t DEFAULT_SWAPCHAIN_IMAGE_COUNT = 2;
        uint32_t DEFAULT_VIEWPORT_POSITION_X = 0.0f;
        uint32_t DEFAULT_VIEWPORT_POSITION_Y = 0.0f;
        uint32_t DEFAULT_VIEWPORT_MINDEPTH = 0.0f;
        uint32_t DEFAULT_VIEWPORT_MAXDEPTH = 1.0f;
        uint32_t DEFAULT_SCISSOR_OFFSET_X = 0;
        uint32_t DEFAULT_SCISSOR_OFFSET_Y = 0;
        uint32_t DEFAULT_VIEWPORT_COUNT = 1;
        uint32_t DEFAULT_SCISSOR_COUNT = 1;
        uint32_t DEFAULT_RASTERIZATION_LINE_WIDTH = 1.0f;
        uint32_t DEFAULT_MIN_SAMPLE_SHADING = 1.0f;
        uint32_t DEFAULT_DEPTHSTENCIL_MINDEPTH = 0.0f;
        uint32_t DEFAULT_DEPTHSTENCIL_MAXDEPTH = 1.0f;
    }
}