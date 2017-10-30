#pragma once
#include "stdafx.h"
#include "ThroneFramework.h"

using namespace R::S::ErrorMessage;
using namespace R::S::Path;
using namespace R::S::StandardOutput;
using namespace R::C;

using std::cout;

namespace ThroneEngine
{
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    Window* window;

    VkPresentModeKHR desiredPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    VkPresentModeKHR presentMode;
    VkQueue selectedQueue;
    VkCommandPool commandPool;

    bool initializeThroneFramework()
    {
        if (initializeVulkanFunctions() == false)
        {
            return false;
        }

        std::vector<VkExtensionProperties> availableExtensions;

        if (checkAvailableExtensions(true, availableExtensions) == false)
        {
            return false;
        }
        std::vector<const char*> requiredExtensions;
        std::vector<const char*> unsupportedExtensions;

        addWin32SurfaceExtensions(requiredExtensions); /////// WIN32

        if (verifyRequiredExtensions(availableExtensions, requiredExtensions, unsupportedExtensions))
        {
            if (createVulkanInstance(requiredExtensions))
            {
                initializeInstanceLevelVulkanFunctions(instance, requiredExtensions);
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        window->initialize(instance);

        std::vector<VkPhysicalDevice> physicalDevices;
        if (enumeratePhysicalDevices(physicalDevices) == false)
        {
            return false;
        }

        std::vector<char const*> requiredDeviceExtensions;
        addSwapchainExtension(requiredDeviceExtensions); /////////// Swapchains
        std::vector<QueueInfo> queueInfos;
        VkPhysicalDeviceFeatures requiredFeatures;
        setFeaturesToFalse(requiredFeatures);
        requiredFeatures.geometryShader = VK_TRUE;
        physicalDevice = findSuitablePhysicalDevice(physicalDevices, requiredDeviceExtensions, requiredFeatures, queueInfos, window->surface);
        if (physicalDevice == VK_NULL_HANDLE)
        {
            return false;
        }

        std::vector<VkPresentModeKHR> availablePresentaionModes;
        if (getAvailablePresentationModes(physicalDevice, window->surface, availablePresentaionModes))
        {
            if (verifyDesiredPresentationModeIsAvailable(availablePresentaionModes, desiredPresentMode) == true)
            {
                presentMode = desiredPresentMode;
            }
            else
            {
                presentMode = VK_PRESENT_MODE_FIFO_KHR;
            }
        }
        else
        {
            return false;
        }

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        if (getSurfaceCapabilities(physicalDevice, window->surface, surfaceCapabilities) == false)
        {
            return false;
        }

        uint32_t numberOfImages = surfaceCapabilities.minImageCount + 1;
        if (surfaceCapabilities.maxImageCount > 0 && numberOfImages > surfaceCapabilities.maxImageCount)
        {
            numberOfImages = surfaceCapabilities.maxImageCount;
        }

        VkExtent2D imageSize;
        if (surfaceCapabilities.currentExtent.width != 0xFFFFFFFF)
        {
            imageSize = surfaceCapabilities.currentExtent;
        }
        else
        {
            ERROR_MESSAGE(err_unSupportedOperation + std::to_string(__LINE__)); // See page 96 vulkan cookbook
        }

        if (createLogicalDevice(logicalDevice, physicalDevice, requiredDeviceExtensions, queueInfos, requiredFeatures) == false)
        {
            return false;
        }

        initializeDeviceLevelVulkanFunctions(logicalDevice, requiredDeviceExtensions);

        vkGetDeviceQueue(logicalDevice, 0, 0, &selectedQueue);

        window->initSurface(instance, physicalDevice);
        window->initSwapChain(logicalDevice, physicalDevice);
        window->initSwapChainImages(logicalDevice);

        createGraphicsPipeline();

        return true;
    }

    inline void destroyThroneFramework()
    {
        if (logicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(logicalDevice, nullptr);
            logicalDevice = VK_NULL_HANDLE;
        }
        if (instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(instance, nullptr);
            instance = VK_NULL_HANDLE;
        }

        window->deInitSwapChainImages(logicalDevice);
        window->deInitSwapChain(logicalDevice);
        window->deInitSurface(instance);
    }

    inline bool createVulkanInstance(std::vector<const char*>& requiredExtensions)
    {
        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = nullptr;
        appInfo.pApplicationName = sdo_windowName.data();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = sdo_windowName.data();
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.enabledExtensionCount = requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.size() > 0 ? &requiredExtensions[0] : nullptr;

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS || instance == VK_NULL_HANDLE)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_VulkanInstance);
            #endif
            return false;
        }
        #endif
        return true;
    }

    inline bool createGraphicsPipeline()
    {
        std::vector<char> vertShaderCode = readFile(path_triangleShaderVert);
        std::vector<char> fragShaderCode = readFile(path_triangleShaderFrag);
        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo;
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = sdo_shaderMethodToInvoke.c_str();

        VkPipelineShaderStageCreateInfo fragShaderStageInfo;
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = sdo_shaderMethodToInvoke.c_str();

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo;
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly;
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport;
        viewport.x = DEFAULT_VIEWPORT_POSITION_X;
        viewport.y = DEFAULT_VIEWPORT_POSITION_Y;
        viewport.width = (float)window->swapChainExtent.width;
        viewport.height = (float)window->swapChainExtent.height;
        viewport.minDepth = DEFAULT_VIEWPORT_MINDEPTH;
        viewport.maxDepth = DEFAULT_VIEWPORT_MAXDEPTH;

        VkRect2D scissor;
        scissor.offset = { DEFAULT_SCISSOR_OFFSET_X, DEFAULT_SCISSOR_OFFSET_Y };
        scissor.extent = window->swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState;
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = DEFAULT_VIEWPORT_COUNT;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = DEFAULT_SCISSOR_COUNT;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer;
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = DEFAULT_RASTERIZATION_LINE_WIDTH;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling;
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = DEFAULT_MIN_SAMPLE_SHADING;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencil;
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = DEFAULT_DEPTHSTENCIL_MINDEPTH;
        depthStencil.maxDepthBounds = DEFAULT_DEPTHSTENCIL_MAXDEPTH;
        depthStencil.stencilTestEnable = VK_FALSE;

        vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
        return true;
    }

    inline VkShaderModule createShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            ERROR_MESSAGE(err_shaderModuleCreation);
        }

        return shaderModule;
    }
    
    inline bool checkAvailableExtensions(const bool mustOutputExtensions, std::vector<VkExtensionProperties>& availableExtensions)
    {
        uint32_t extensionsCount;
        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS || extensionsCount == 0)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_extensionNumber);
            #endif
            return false;
        }
        #endif

        availableExtensions.resize(extensionsCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, &availableExtensions[0]);

        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS || availableExtensions.size() == 0 || availableExtensions.size() != extensionsCount)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_extensionEnumeration);
            #endif
            return false;
        }
        #endif

        if (mustOutputExtensions == true)
        {
            enumerateExtensions(availableExtensions);
        }

        return true;
    }

    inline bool verifyRequiredExtensions(std::vector<VkExtensionProperties>& availableExtensions, std::vector<const char*>& requiredExtensions, std::vector<const char*>& unsupportedExtensions)
    {
        for (const char* reqExtension : requiredExtensions)
        {
            if (isExtensionSupported(availableExtensions, reqExtension) == false)
            {
                unsupportedExtensions.push_back(reqExtension);
                #if ERROR_MESSAGE_NEEDED
                ERROR_MESSAGE(err_unSupportedExtension + std::string(reqExtension));
                #endif
            }
        }
        if (unsupportedExtensions.size() > 0)
        {
            return false;
        }
        return true;
    }

    void enumerateExtensions(std::vector<VkExtensionProperties>& availableExtensions)
    {
        cout << sdo_extensionNumber << availableExtensions.size() << "\n";
        for (int i = 0; i < availableExtensions.size(); i++)
        {
            cout << sdo_extension << availableExtensions[i].extensionName << "\n";
        }
    }

    inline bool enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& physicalDevices)
    {
        uint32_t devicesCount;
        VkResult result = vkEnumeratePhysicalDevices(instance, &devicesCount, nullptr);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_physicalDevicesNumber);
            #endif
            return false;
        }
        #endif
        physicalDevices.resize(devicesCount);
        result = vkEnumeratePhysicalDevices(instance, &devicesCount, &physicalDevices[0]);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_physicalDevicesEnumerate);
            #endif
            return false;
        }
        #endif
        return true;
    }

    inline VkPhysicalDevice findSuitablePhysicalDevice(
        std::vector<VkPhysicalDevice>& physicalDevices,
        std::vector<const char*>& requiredDeviceExtensions,
        VkPhysicalDeviceFeatures& requiredFeatures,
        std::vector<QueueInfo>& queueInfos,
        VkSurfaceKHR surface)
    {
        for (VkPhysicalDevice& device : physicalDevices)
        {
            std::vector<VkExtensionProperties> availableExtensions;
            if (enumerateAvailableExtensionsOnDevice(device, availableExtensions) == true)
            {
                if (verifyRequiredExtensions(availableExtensions, requiredDeviceExtensions, std::vector<const char*>()))
                {
                    VkPhysicalDeviceFeatures features;
                    vkGetPhysicalDeviceFeatures(device, &features);

                    if (requiredFeaturesAreSupported(requiredFeatures, features) == true)
                    {
                        VkPhysicalDeviceProperties properties;
                        vkGetPhysicalDeviceProperties(device, &properties);

                        std::vector<VkQueueFamilyProperties> queueFamilies;
                        if (enumerateDeviceQueueFamilies(device, queueFamilies) == true)
                        {
                            queueInfos.push_back(QueueInfo());
                            queueInfos[0].familyIndex = 0; ////////////// HARDCODE
                            queueInfos[0].priorities.push_back(1.0f);
                            int presentationQueueIndex = findSuitableQueueFamilyForSwapchainPresentation(device, queueFamilies, surface);
                            if (presentationQueueIndex != -1)
                            {
                                return device;
                            }
                        }
                    }
                }
            }
        }
        #if ERROR_MESSAGE_NEEDED
        ERROR_MESSAGE(err_physicalDevicesSuitable);
        #endif
        return nullptr;
    }

    inline bool enumerateAvailableExtensionsOnDevice(VkPhysicalDevice& device, std::vector<VkExtensionProperties>& availableExtensions)
    {
        uint32_t extensionCount;
        VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS || extensionCount <= 0)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_deviceExtensionNumber);
            #endif
            return false;
        }
        #endif

        availableExtensions.resize(extensionCount);
        result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, &availableExtensions[0]);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS || device == VK_NULL_HANDLE)
        {
            #if ERROR_MESSAGE_NEEDED	
            ERROR_MESSAGE(err_physicalDevice);
            #endif
            return false;
        }
        #endif
        return true;
    }

    inline bool requiredFeaturesAreSupported(VkPhysicalDeviceFeatures& requiredFeatures, VkPhysicalDeviceFeatures& availableFeatures)
    {
        int s = sizeof(VkPhysicalDeviceFeatures) / sizeof(uint32_t);
        unsigned int* requiredAdress = (uint32_t*)&requiredFeatures;
        unsigned int* availableAdress = (uint32_t*)&availableFeatures;
        for (int i = 0; i < s; i++)
        {
            uint32_t* requiredValue = requiredAdress + i;
            if (*requiredValue == 1)
            {
                uint32_t* availableValue = availableAdress + i;
                if (*availableValue != *requiredValue)
                {
                    return false;
                }
            }
        }
        return true;
    }

    inline bool enumerateDeviceQueueFamilies(VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queueFamilies)
    {
        uint32_t queueFamiliesCount;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, nullptr);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (queueFamiliesCount <= 0)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_QueueFamilyNumber);
            #endif
            return false;
        }
        #endif
        queueFamilies.resize(queueFamiliesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, &queueFamilies[0]);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (queueFamilies.size() != queueFamiliesCount)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_QueueFamily);
            #endif
            return false;
        }
        #endif

        return true;
    }

    inline int findSuitableQueueFamily(std::vector<VkQueueFamilyProperties>& queueFamilies, VkQueueFlags desiredCapabilities)
    {
        int size = queueFamilies.size();
        for (int i = 0; i < size; i++)
        {
            if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & desiredCapabilities)
            {
                return i;
            }
        }
        return -1;
    }

    inline bool createLogicalDevice(
        VkDevice& logicalDevice,
        VkPhysicalDevice& physicalDevice,
        std::vector<char const*>& requiredDeviceExtensions,
        std::vector<QueueInfo>& queueInfos,
        VkPhysicalDeviceFeatures& requiredFeatures)
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        for (int i = 0; i < queueInfos.size(); i++)
        {
            VkDeviceQueueCreateInfo graphicsQueue;
            graphicsQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            graphicsQueue.pNext = nullptr;
            graphicsQueue.flags = 0;
            graphicsQueue.queueFamilyIndex = queueInfos[i].familyIndex;
            graphicsQueue.pQueuePriorities = &queueInfos[i].priorities[0];
            graphicsQueue.queueCount = queueInfos[i].priorities.size();
            queueCreateInfos.push_back(graphicsQueue);
        }

        VkDeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.size() > 0 ? &queueCreateInfos[0] : nullptr;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.enabledExtensionCount = requiredDeviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions.size() > 0 ? &requiredDeviceExtensions[0] : nullptr;
        deviceCreateInfo.pEnabledFeatures = &requiredFeatures;

        VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS)
        {
            #if ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_logicalDeviceCreation);
            #endif 
            return false;
        }
        #endif 
        return true;
    }

    inline int findSuitableQueueFamilyForSwapchainPresentation(VkPhysicalDevice physicalDevice, std::vector<VkQueueFamilyProperties>& queueFamilies, VkSurfaceKHR surface)
    {
        for (int i = 0; i < queueFamilies.size(); i++)
        {
            VkBool32 presentationAvailable;
            VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentationAvailable);
            #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
            if (result != VK_SUCCESS)
            {
                #if defined ERROR_MESSAGE_NEEDED
                ERROR_MESSAGE(err_physicalDeviceSupportForQueue);
                #endif 
            }
            #endif
            if (presentationAvailable == VK_TRUE)
            {
                return i;
            }
        }
        #if defined ERROR_MESSAGE_NEEDED
        ERROR_MESSAGE(err_queueFamilySupportForPresentation);
        #endif 
        return -1;
    }

    inline bool getAvailablePresentationModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        uint32_t presentationModeCount;
        VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationModeCount, nullptr);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS)
        {
            #if defined ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_deviceSurfaceNumber);
            #endif 		
            return false;
        }
        #endif

        availablePresentModes.resize(presentationModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationModeCount, &availablePresentModes[0]);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS)
        {
            #if defined ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_availablePresentationModes);
            #endif 		
            return false;
        }
        #endif
        return true;
    }

    inline bool verifyDesiredPresentationModeIsAvailable(std::vector<VkPresentModeKHR>& availablePresentModes, VkPresentModeKHR desiredPresentMode)
    {
        for (int i = 0; i < availablePresentModes.size(); i++)
        {
            if (availablePresentModes[i] == desiredPresentMode)
            {
                return true;
            }
        }
        return false;
    }

    inline bool getSurfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& surfaceCapabilities)
    {
        VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS)
        {
            #if defined ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_physicalDeviceCapabality);
            #endif
            return false;
        }
        #endif
        return true;
    }

    inline void setFeaturesToFalse(VkPhysicalDeviceFeatures& features)
    {
        int s = sizeof(VkPhysicalDeviceFeatures) / sizeof(uint32_t);
        uint32_t * adress = (uint32_t*)&features;
        for (int i = 0; i < s; i++)
        {
            uint32_t* value = adress + i;
            *value = VK_FALSE;
        }
    }

    inline bool createCommandPool(VkDevice logicalDevice, QueueInfo& queueInfo, VkCommandPool commandPool, VkFlags parameters)
    {
        VkCommandPoolCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = parameters;
        createInfo.queueFamilyIndex = queueInfo.familyIndex;

        VkResult result = vkCreateCommandPool(logicalDevice, &createInfo, nullptr, &commandPool);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS)
        {
            #if defined ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_commandPoolCreation);
            #endif
            return false;
        }
        #endif
        return true;
    }

    inline bool createCommandBuffers(VkDevice logicalDevice, VkCommandPool commandPool, std::vector<VkCommandBuffer>& commandBuffers, const int numberOfBufferToCreate)
    {
        commandBuffers.resize(numberOfBufferToCreate);
        VkCommandBufferAllocateInfo allocateInfo;
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.pNext = nullptr;
        allocateInfo.commandPool = commandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = numberOfBufferToCreate;

        VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocateInfo, &commandBuffers[0]);
        #if defined ERROR_VALIDATION_VKSUCCESS_CHECK
        if (result != VK_SUCCESS)
        {
            #if defined ERROR_MESSAGE_NEEDED
            ERROR_MESSAGE(err_commandBuffer);
            #endif
            return false;
        }
        #endif
        return true;
    }

    inline bool submitRequestToBuffer()
    {
        return true;
    }

    inline std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        std::vector<char> buffer;

        if (!file.is_open())
        {
            ERROR_MESSAGE(err_fileOpen);
        }
        else
        {
            size_t fileSize = (size_t)file.tellg();
            buffer.resize(fileSize);
            file.seekg(0);
            file.read(buffer.data(), fileSize);
            file.close();
        }
        return buffer;
    }


    #pragma region Temporaire

    #if defined VK_USE_PLATFORM_WIN32_KHR
    inline void addWin32SurfaceExtensions(std::vector<const char*>& requiredExtensions)
    {
        requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        requiredExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    }
    #endif
    inline void addSwapchainExtension(std::vector<const char*>& requiredExtensions)
    {
        requiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }
    #pragma endregion



}