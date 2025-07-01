//Copyright (C) 2023 Alrick Grandison, Algodal™

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vk-std.h"
#include "vk-utils.h"
#include "vk-global.h"
#include "global.h"


void CreateInstance(char* name, uint32_t version)
{
    const char* names_layer[] = {
        //#ifdef DEBUG
        "VK_LAYER_KHRONOS_validation"
        //#endif
    };
    const uint32_t count_names_layer = sizeof(names_layer) / sizeof(char*);

    DEFINE_INSTANCE_EXTENSIONS(
        window, 
        count_instance_extensions, 
        names_instance_extensions, 
        8
        , "VK_KHR_surface"
        #ifdef DEBUG
        , "VK_EXT_debug_utils"
        #endif
    );

    VkApplicationInfo info_app = {0};
    info_app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info_app.apiVersion = VK_API_VERSION_1_3;
    info_app.pEngineName = "C SDL2 Vulkan";
    info_app.pApplicationName = name;
    info_app.applicationVersion = version;

    VkInstanceCreateInfo info_create = {0};
    info_create.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info_create.pApplicationInfo = &info_app;
    info_create.enabledExtensionCount = count_instance_extensions;
    info_create.ppEnabledExtensionNames = names_instance_extensions;
    info_create.enabledLayerCount = count_names_layer;
    info_create.ppEnabledLayerNames = names_layer;
    #ifdef DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debug_info = GetDebugUtilsMessengerInfo();
    info_create.pNext = &debug_info;
    #endif

    EXIT_VK_RESULT(vkCreateInstance(&info_create, 0, &instance), "Failed to create Vulkan Instance");
}

void DestroyInstance()
{
    vkDestroyInstance(instance, 0);
    instance = 0;
}

void CreateSurface() 
{
    if( SDL_Vulkan_CreateSurface(window, instance, NULL, &surface) == false) {
        SDL_Log("Failed to create Vulkan Surface");
        exit(EXIT_FAILURE);
    }
}

void DestroySurface() 
{
    vkDestroySurfaceKHR(instance, surface, 0);
}

void CreateDevice() 
{
    DEFINE_ENUMERATE(
        VkPhysicalDevice, 
		vkEnumeratePhysicalDevices,
		instance, 
		count_physicaldevices, 
		physicaldevices, 16
	);

	uint32_t score_max = 0;

	for (uint32_t i = 0; i < count_physicaldevices; i++) {
		VkPhysicalDeviceProperties ppd = {0};
		vkGetPhysicalDeviceProperties(physicaldevices[i], &ppd);
		DEFINE_ENUMERATE(
			VkQueueFamilyProperties,
			vkGetPhysicalDeviceQueueFamilyProperties, 
			physicaldevices[i],
			queuefamily_properties_count, 
			queuefamily_properties, 
			32
		);

		uint32_t score = GetPhysicalDeviceScore(ppd.limits);

		for (uint32_t j = 0; j < queuefamily_properties_count; j++) {
			if (queuefamily_properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				VkBool32 supported = VK_FALSE;
				vkGetPhysicalDeviceSurfaceSupportKHR(
					physicaldevices[i], j,
					surface, 
					&supported
				);

				if (supported && (score > score_max)) {
					physicaldevice = physicaldevices[i];
					index_queuefamily = j;
					properties_physicaldevice = ppd;
					vkGetPhysicalDeviceMemoryProperties(
						physicaldevice,
						&properties_physicaldevice_memory
					);
                    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                        physicaldevice,
                        surface,
                        &capabilities_surface
                    );

					score_max = score;
				}
			}
		}
	}

	if(physicaldevice == VK_NULL_HANDLE ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to find Vulkan Physical Device");
        exit(EXIT_FAILURE);
    }

    const char* names_extension[] = {
        "VK_KHR_swapchain"
    };

    #define count_extension (sizeof(names_extension) / sizeof(char*))

    if(count_extension > 1)
    {
        #define max_available_extensions 64
        uint32_t count_available_extensions = 0; 
        VkExtensionProperties properties_available_extensions[max_available_extensions];

        vkEnumerateDeviceExtensionProperties(physicaldevice, 0, &count_available_extensions, 0 ); 
        count_available_extensions = count_available_extensions > max_available_extensions ? max_available_extensions : count_available_extensions;
        vkEnumerateDeviceExtensionProperties(physicaldevice, 0, &count_available_extensions, properties_available_extensions );
        
        bool found_ext[count_extension];
        bool any_not_found = false;
        for(uint32_t i = 1; i < count_extension; i++)
        {
            found_ext[i] = false;
            for(uint32_t j = 0; j < count_available_extensions; j++)
            {
                if(strcmp(names_extension[i], properties_available_extensions[j].extensionName) == 0)
                {
                    found_ext[i] = true;
                    break;
                }
            }

            if(found_ext[i] == false)
            {
                any_not_found = true;
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s extension is not found", names_extension[i]);
            }
        }
        if(any_not_found)
        {
            exit(EXIT_FAILURE);
        }
    }

    float priority = 1.0f;

    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueCount = 1;
    queue_create_info.queueFamilyIndex = index_queuefamily;
    queue_create_info.pQueuePriorities = &priority;

    VkDeviceCreateInfo device_create_info = {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.enabledExtensionCount = count_extension;
    device_create_info.ppEnabledExtensionNames = names_extension;

    EXIT_VK_RESULT(vkCreateDevice(physicaldevice, &device_create_info, 0, &device), "Failed Vulkan Device Creation");

    vkGetDeviceQueue(device, index_queuefamily, 0, &queue);
}

void DestroyDevice() 
{
    vkDestroyDevice(device, 0);
}

void CreateRenderPass() 
{
    VkSurfaceFormatKHR surface_format = GetAvailableSurfaceFormat(DEFAULT_SURFACE_FORMAT);

    VkAttachmentDescription desc_attachment = {};
    desc_attachment.format = surface_format.format;
    desc_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    desc_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    desc_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    desc_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    desc_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    desc_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription descriptions[] = {
        desc_attachment
    };

    VkAttachmentReference ref_attachment = {};
    ref_attachment.attachment = 0;
    ref_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference references[] = {
        ref_attachment
    };

    VkSubpassDescription desc_subpass = {};
    desc_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    desc_subpass.colorAttachmentCount = sizeof(references) / sizeof(VkAttachmentReference);
    desc_subpass.pColorAttachments = references;   

    VkSubpassDescription subpasses[] = {
        desc_subpass
    };

    VkRenderPassCreateInfo info_create = {};
    info_create.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info_create.attachmentCount = sizeof(descriptions) / sizeof(VkAttachmentDescription);
    info_create.pAttachments = descriptions;
    info_create.subpassCount = sizeof(subpasses) / sizeof(VkSubpassDescription);
    info_create.pSubpasses = subpasses;

    EXIT_VK_RESULT(vkCreateRenderPass(device, &info_create, 0, &renderpass), "Failed to create Vulkan Render Pass");
}

void DestroyRenderPass() 
{
    vkDestroyRenderPass(device, renderpass, 0);
}

void CreateSwapchain() 
{
    VkSurfaceFormatKHR surface_format = GetAvailableSurfaceFormat(DEFAULT_SURFACE_FORMAT);

    VkSwapchainCreateInfoKHR info_create = {};
    info_create.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info_create.surface = surface;
    info_create.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info_create.minImageCount = GetAvailableSwapchainSize(DEFAULT_SWAPCHAIN_SIZE);
    info_create.imageFormat = surface_format.format;
    info_create.imageExtent = capabilities_surface.currentExtent;
    info_create.imageArrayLayers = 1;
    info_create.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info_create.imageColorSpace = surface_format.colorSpace;
    info_create.preTransform = capabilities_surface.currentTransform;
    info_create.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info_create.presentMode = GetAvailablePresentMode(DEFAULT_PRESENT_MODE);
    info_create.clipped = VK_TRUE;

    if(ConfirmVkResult(vkCreateSwapchainKHR(device, &info_create, 0, &swapchain)) == 0) {
        SDL_Log("Failed Vulkan Swapchain Creation");
        exit(EXIT_FAILURE);
    }

    DEFINE_ENUMERATE_2(
        VkImage, 
        vkGetSwapchainImagesKHR, 
        device, 
        swapchain, 
        images_count, 
        images, 
        16
    );

    size_swapchain = images_count;
    images_swapchain = images;
    imageviews_swapchain = malloc(sizeof(VkImageView) * images_count);
    framebuffers_swapchain = malloc(sizeof(VkFramebuffer) * images_count);

    for (uint32_t i = 0; i < images_count; i++) {
        VkImageSubresourceRange subresource = {};
        subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresource.levelCount = 1;
        subresource.layerCount = 1;

        VkImageViewCreateInfo view_create_info = {};
        view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_create_info.image = images[i];
        view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_create_info.format = surface_format.format;
        view_create_info.subresourceRange = subresource;

        if(ConfirmVkResult(vkCreateImageView(device, &view_create_info, 0, &imageviews_swapchain[i])) == 0) {
            SDL_Log("Failed to create Vulkan Swapchain Image View");
            exit(EXIT_FAILURE);
        }
    }

    for (uint32_t i = 0; i < images_count; i++) {
        VkImageView attachments[] = {
            imageviews_swapchain[i]
        };
        VkFramebufferCreateInfo fb_create_info = {};
        fb_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_create_info.renderPass = renderpass;
        fb_create_info.attachmentCount = sizeof(attachments) / sizeof(VkImageView);
        fb_create_info.pAttachments = attachments;
        fb_create_info.width = WIDTH;
        fb_create_info.height = HEIGHT;
        fb_create_info.layers = 1;

        if(ConfirmVkResult(vkCreateFramebuffer(device, &fb_create_info, 0, &framebuffers_swapchain[i])) == 0) {
            SDL_Log("Failed to create Vulkan Swapchain Framebuffer");
            exit(EXIT_FAILURE);
        }
    }
}

void DestroySwapchain() 
{
    for(uint32_t i = 0; i < size_swapchain; i++) {
        vkDestroyFramebuffer(device, framebuffers_swapchain[i], 0);
    }
    free(framebuffers_swapchain);

    for(uint32_t i = 0; i < size_swapchain; i++) {
        vkDestroyImageView(device, imageviews_swapchain[i], 0);
    }
    free(imageviews_swapchain);

    vkDestroySwapchainKHR(device, swapchain, 0);
}

void CreateCommandBuffers() 
{
    VkCommandPoolCreateInfo info_create = {};
    info_create.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info_create.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info_create.queueFamilyIndex = index_queuefamily;

    EXIT_VK_RESULT(vkCreateCommandPool(device, &info_create, 0, &commandpool), "Failed Vulkan Commandpool Creation");

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandpool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = sizeof(commandbuffers) / sizeof(VkCommandBuffer);

    EXIT_VK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, commandbuffers), "Failed Vulkan Commandbuffers Allocation");
}

void DestroyCommandBuffers() 
{
    vkDestroyCommandPool(device, commandpool, 0);
}

void CreatePipelineLayout() 
{
    VkDescriptorSetLayoutBinding bindings[2] = {};

    // Binding 0
    // Uniform Buffers
    //---------------------------------

    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;    
    bindings[0].binding = 0; // shader binding point    
    bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[0].descriptorCount = 1; // number of elements in binding

    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;    
    bindings[1].binding = 1; // shader binding point    
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[1].descriptorCount = 1; // number of elements in binding

    VkDescriptorSetLayoutCreateInfo info_descriptorsetlayout = {};
    info_descriptorsetlayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info_descriptorsetlayout.bindingCount = sizeof(bindings) / sizeof(VkDescriptorSetLayoutBinding);
    info_descriptorsetlayout.pBindings = bindings;

    if(ConfirmVkResult(vkCreateDescriptorSetLayout(device, &info_descriptorsetlayout, 0, &descriptorsetlayout)) == 0) {
        SDL_Log("Failed to create Vulkan Descriptor Set Layout");
        exit(EXIT_FAILURE);
    }

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(VkExtent2D);

    VkPipelineLayoutCreateInfo pipeline_layout_info = {};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 0;
    pipeline_layout_info.pSetLayouts = &descriptorsetlayout;
    pipeline_layout_info.pushConstantRangeCount  = 0;
	pipeline_layout_info.pPushConstantRanges = &pushConstantRange;

    if(ConfirmVkResult(vkCreatePipelineLayout(device, &pipeline_layout_info, 0, &pipelinelayout)) == 0) {
        SDL_Log("Failed to create Vulkan pipeline layout");
        exit(EXIT_FAILURE);
    }
}

void DestroyPipelineLayout() 
{
    vkDestroyDescriptorSetLayout(device, descriptorsetlayout, 0);
    vkDestroyPipelineLayout(device, pipelinelayout, 0);
}

void CreatePipeline() 
{
    VkShaderModule shader_vertex = CreateShaderModule("./vert.spv");
    VkShaderModule shader_fragment = CreateShaderModule("./frag.spv");

    VkPipelineShaderStageCreateInfo vertex_create_info = {};
    vertex_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertex_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertex_create_info.module = shader_vertex;
    vertex_create_info.pName = "main";

    VkPipelineShaderStageCreateInfo fragment_create_info = {};
    fragment_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragment_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragment_create_info.module = shader_fragment;
    fragment_create_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_create_infos[] = {
        vertex_create_info, 
        fragment_create_info
    };
    const uint32_t shader_create_infos_count = 
        sizeof(shader_create_infos) / sizeof(VkPipelineShaderStageCreateInfo);
    
    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {};
    dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.dynamicStateCount = sizeof(dynamic_states) / sizeof(VkDynamicState);
    dynamic_state_create_info.pDynamicStates = dynamic_states;

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {};
    rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
    rasterization_state_create_info.cullMode = VK_CULL_MODE_NONE;
    rasterization_state_create_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterization_state_create_info.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {};
    multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
    color_blend_attachment_state.colorWriteMask = 
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT
    ;

    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {};
    color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state_create_info.attachmentCount = 1;
    color_blend_state_create_info.pAttachments = &color_blend_attachment_state;

    VkPipelineViewportStateCreateInfo viewport_state_create_info = {};
    viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_create_info.viewportCount = 1;
    viewport_state_create_info.scissorCount = 1;

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {};
    vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {};
    input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkGraphicsPipelineCreateInfo pipeline_create_info = {};
    pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_create_info.layout = pipelinelayout;
    pipeline_create_info.stageCount = shader_create_infos_count;
    pipeline_create_info.pStages = shader_create_infos;
    pipeline_create_info.pDynamicState = &dynamic_state_create_info;
    pipeline_create_info.renderPass = renderpass;
    pipeline_create_info.pRasterizationState = &rasterization_state_create_info;
    pipeline_create_info.pMultisampleState = &multisample_state_create_info;
    pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
    pipeline_create_info.pViewportState = &viewport_state_create_info;
    pipeline_create_info.pVertexInputState = &vertex_input_state_create_info;
    pipeline_create_info.pInputAssemblyState = &input_assembly_state_create_info;

    EXIT_VK_RESULT(vkCreateGraphicsPipelines(device, 0, 1, &pipeline_create_info, 0, &pipeline), "Failed to create Vulkan Graphics Pipeline");

    vkDestroyShaderModule(device, shader_vertex, 0);
    vkDestroyShaderModule(device, shader_fragment, 0);
}

uint32_t GetNextDrawImageIndex() {
    uint32_t image_index = 0;
    vkAcquireNextImageKHR(
        device, 
        swapchain, 
        UINT32_MAX, 
        VK_NULL_HANDLE, 
        fence, 
        &image_index
    );
    return image_index;
}

VkBool32 RecordCommands(uint32_t index_image) {
    VkClearValue clear_value = {0.0f, 0.0f, 0.4, 1.0f};

    VkRect2D scissor = {};
    scissor.extent = capabilities_surface.currentExtent;

    VkViewport viewport = {};
    viewport.width = (float)capabilities_surface.currentExtent.width;
    viewport.height = (float)capabilities_surface.currentExtent.height;
    viewport.maxDepth = 1.0f;

    VkRenderPassBeginInfo render_pass_begin_info = {};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = renderpass;
    render_pass_begin_info.framebuffer = framebuffers_swapchain[index_image];
    render_pass_begin_info.renderArea.extent = capabilities_surface.currentExtent;
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_value;

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    // Recording Commands
    //------------------------

    RETURN_VK_RESULT(vkBeginCommandBuffer(draw_commandbuffer, &begin_info), "Failed to Begin Recording Command Buffer", VK_FALSE);   
    vkCmdBeginRenderPass(draw_commandbuffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(draw_commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    vkCmdSetViewport(draw_commandbuffer, 0, 1, &viewport);
    vkCmdSetScissor(draw_commandbuffer, 0, 1, &scissor);
    vkCmdDraw(draw_commandbuffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(draw_commandbuffer);
    RETURN_VK_RESULT(vkEndCommandBuffer(draw_commandbuffer), "Failed to End Recording Command Buffer", VK_FALSE);

    return VK_TRUE;
}

void SubmitCommands(uint32_t index_image) {
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &draw_commandbuffer;

    RETURN_VK_RESULT(vkQueueSubmit(queue, 1, &submit_info, 0), "Failed to Submit Command to Queue");
    vkQueueWaitIdle(queue);

    VkSwapchainKHR swapChains[] = {swapchain};

    VkPresentInfoKHR present_info = {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapChains;
    present_info.pImageIndices = &index_image;

    RETURN_VK_RESULT(vkQueuePresentKHR(queue, &present_info), "Failed to Present Queue");
    vkQueueWaitIdle(queue);
}











