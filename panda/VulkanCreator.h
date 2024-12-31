#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define CATCH(value) if(value != VK_SUCCESS){ std::cout << "Error!"; } //Check if an error is happening.

#include <iostream>
#include <vector>

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

struct GameInfo
{
	char* gameName; // The name of the Game.

	uint32_t gameVersion; // The current version.

	VkExtent2D getWindowScale; // The default scale of the window.
};

