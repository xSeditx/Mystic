#pragma once



#include<string>
#include<vector>

struct VKAppInfo
{
	std::string Type;
	std::string	ApplicationName;
	unsigned int ApplicationVersion;
	std::string EngineName;
	unsigned int EngineVersion;
	unsigned int ApiVersion;
};



void createInstance() {
	VKAppInfo AppInfo = {};
	///AppInfo.Type = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.ApplicationName = "Hello Triangle";
	///AppInfo.ApplicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.EngineName = "No Engine";
//	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.ApiVersion = 1; //VK_API_VERSION_1_0;
}



/// Check for VK Extensions

// class VkExtensionProperties;
// std::vector<VkExtensionProperties> GetVulkanExtensions()
// {
// //uint32_t extensionCount = 0;
// //vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
// //std::vector<VkExtensionProperties> extensions(extensionCount);
// //vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
// //std::cout << "available extensions:" << std::endl;
// //for (const auto& extension : extensions)
// //{
// //	std::cout << "\t" << extension.extensionName << std::endl;
// //}
// }

/// Create Validation Layers

///Checking the values of parameters against the specification to detect misuse
///Tracking creation and destruction of objects to find resource leaks
///Checking thread safety by tracking the threads that calls originate from
///Logging every call and its parameters to the standard output
///Tracing Vulkan calls for profiling and replaying
// bool checkValidationLayerSupport() {
// 	uint32_t layerCount;
// 	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
// 
// 	std::vector<VkLayerProperties> availableLayers(layerCount);
// 	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
// 
// 	return false;
// }
//  VkResult vkCreateInstance(
//  	const VkInstanceCreateInfo* pCreateInfo,
//  	const VkAllocationCallbacks* pAllocator,
//  	VkInstance* instance) {
//  
//  	if (pCreateInfo == nullptr || instance == nullptr) {
//  		log("Null pointer passed to required parameter!");
//  		return VK_ERROR_INITIALIZATION_FAILED;
//  	}
//  
//  	return real_vkCreateInstance(pCreateInfo, pAllocator, instance);
//  }

//
// 
//
//class ValidationLayer
//{
//	VkLayerProperties Properties;
//
//	VkAllocationCallbacks* Allocator;
//
//
//	bool LayerSupported()
//	{
//		vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
//
//		std::vector<VkLayerProperties> AvailableLayers(LayerCount);
//		vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());
//		return false;
//	}
//
//
//	static std::vector<const char*> AllLayers;
//	static uint32_t LayerCount;
//};
//uint32_t ValidationLayer::LayerCount = 0;
//std::vector<const char*> ValidationLayer::AllLayers = 
//{
//"VK_LAYER_KHRONOS_validation"
//};
//
//
//#ifdef NDEBUG
//const bool enableValidationLayers = false;
//#else
//const bool enableValidationLayers = true;
//#endif
//
//
////class Renderer
//{
//      VkInstance* Instance;
//      VkInstanceCreateInfo* CreateInfo,
////	GLFWWindow Window
//};
//
/// Clean Up Vulkan
//void Renderer::CleanUp() 
//{
//	vkDestroyInstance(Instance, nullptr);
//	glfwDestroyWindow(Window);
//	glfwTerminate();
//}