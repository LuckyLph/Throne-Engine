#pragma once

namespace R
{
    namespace S
    {
        using std::string;

        namespace ErrorMessage
        {
            THRONE_API_ENTRY string err_unManaged;
            THRONE_API_ENTRY string err_unSupportedOperation;
            THRONE_API_ENTRY string err_throneInitialization;
            THRONE_API_ENTRY string err_VulkanInstance;
            THRONE_API_ENTRY string err_extensionNumber;
            THRONE_API_ENTRY string err_extensionEnumeration;
            THRONE_API_ENTRY string err_unSupportedExtension;
            THRONE_API_ENTRY string err_physicalDevicesNumber;
            THRONE_API_ENTRY string err_physicalDevicesEnumerate;
            THRONE_API_ENTRY string err_physicalDevicesSuitable;
            THRONE_API_ENTRY string err_deviceExtensionNumber;
            THRONE_API_ENTRY string err_physicalDevice;
            THRONE_API_ENTRY string err_QueueFamilyNumber;
            THRONE_API_ENTRY string err_QueueFamily;
            THRONE_API_ENTRY string err_logicalDeviceCreation;
            THRONE_API_ENTRY string err_physicalDeviceSupportForQueue;
            THRONE_API_ENTRY string err_queueFamilySupportForPresentation;
            THRONE_API_ENTRY string err_deviceSurfaceNumber;
            THRONE_API_ENTRY string err_availablePresentationModes;
            THRONE_API_ENTRY string err_physicalDeviceCapabality;
            THRONE_API_ENTRY string err_commandPoolCreation;
            THRONE_API_ENTRY string err_commandBuffer;
            THRONE_API_ENTRY string err_vulkanLibrary;
            THRONE_API_ENTRY string err_vulkanLibraryConnect;
            THRONE_API_ENTRY string err_physicalSurfacePresentMode;
            THRONE_API_ENTRY string err_swapChain;
            THRONE_API_ENTRY string err_swapChainImages;
            THRONE_API_ENTRY string err_imageViewCreation;
            THRONE_API_ENTRY string err_fileOpen;
            THRONE_API_ENTRY string err_functionLoad;
            THRONE_API_ENTRY string err_vulkanInstanceFunctionLoad;
            THRONE_API_ENTRY string err_vulkanDeviceFunctionLoad;
            THRONE_API_ENTRY string err_shaderModuleCreation;
        }

        namespace StandardOutput
        {
            THRONE_API_ENTRY string sdo_windowName;
            THRONE_API_ENTRY string sdo_extensionNumber;
            THRONE_API_ENTRY string sdo_extension;
            THRONE_API_ENTRY string sdo_shaderMethodToInvoke;
        }

        namespace Path
        {
            THRONE_API_ENTRY string path_triangleShaderVert;
            THRONE_API_ENTRY string path_triangleShaderFrag;
        }
    }

    namespace C
    {
        THRONE_API_ENTRY uint32_t DEFAULT_WINDOW_WIDTH;
        THRONE_API_ENTRY uint32_t DEFAULT_WINDOW_HEIGTH;
        THRONE_API_ENTRY uint32_t DEFAULT_SWAPCHAIN_IMAGE_COUNT;
        THRONE_API_ENTRY uint32_t DEFAULT_VIEWPORT_POSITION_X;
        THRONE_API_ENTRY uint32_t DEFAULT_VIEWPORT_POSITION_Y;
        THRONE_API_ENTRY uint32_t DEFAULT_VIEWPORT_MINDEPTH;
        THRONE_API_ENTRY uint32_t DEFAULT_VIEWPORT_MAXDEPTH;
        THRONE_API_ENTRY uint32_t DEFAULT_SCISSOR_OFFSET_X;
        THRONE_API_ENTRY uint32_t DEFAULT_SCISSOR_OFFSET_Y;
        THRONE_API_ENTRY uint32_t DEFAULT_VIEWPORT_COUNT;
        THRONE_API_ENTRY uint32_t DEFAULT_SCISSOR_COUNT;
        THRONE_API_ENTRY uint32_t DEFAULT_RASTERIZATION_LINE_WIDTH;
        THRONE_API_ENTRY uint32_t DEFAULT_MIN_SAMPLE_SHADING;
        THRONE_API_ENTRY uint32_t DEFAULT_DEPTHSTENCIL_MINDEPTH;
        THRONE_API_ENTRY uint32_t DEFAULT_DEPTHSTENCIL_MAXDEPTH;
    }
}