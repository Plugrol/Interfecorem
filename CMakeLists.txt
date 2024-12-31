cmake_minimum_required(VERSION 3.10)
project(MyVulkanProject)

# Find the Vulkan package
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Find the GLM package
find_package(glm CONFIG REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the 'panda' folder
include_directories(${PROJECT_SOURCE_DIR}/panda)

# Gather source files from the 'panda' folder
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/panda/*.cpp")

# Add the executable
add_executable(MyVulkanProject ${SOURCES})

# Link Vulkan and GLM libraries
target_link_libraries(MyVulkanProject Vulkan::Vulkan glm::glm glfw)