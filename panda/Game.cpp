#include "Game.h"

Game::Game(const GameInfo& gameInfo) 
{
    this->m_gameInfo = gameInfo;
}

void Game::start() 
{
    this->m_vulkanVK.start(this->m_gameInfo);
    gameLoop();
    shutdown();
}

auto gameStart = std::chrono::high_resolution_clock::now();
void Game::gameLoop()
{
    while (!glfwWindowShouldClose(this->m_vulkanVK.getWindow()))
    {
        glfwPollEvents();

        const float timeEstimated = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - gameStart).count();

        glm::mat4 model = rotate(glm::mat4(1.0f), timeEstimated * glm::radians(0.1f), glm::vec3(0.0f, 0.5f, 2.0f));
        glm::mat4 view = lookAt(glm::vec3(1.0f, 2.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), this->m_gameInfo.getWindowScale.width / static_cast<float>(this->m_gameInfo.getWindowScale.height), 0.01f, 10.0f);
        projection[1][1] *= -1;

        this->m_vulkanVK.m_bufferVK.setMVP(projection * view * model);
        void* data;

        glm::mat4 MVP = this->m_vulkanVK.m_bufferVK.getMVP();
        vkMapMemory(this->m_vulkanVK.m_deviceVK.getDevice(), this->m_vulkanVK.m_bufferVK.getUniformBuffer().bufferMemory, 0, sizeof(MVP), 0, &data);
        memcpy(data, &MVP, sizeof(MVP));
        vkUnmapMemory(this->m_vulkanVK.m_deviceVK.getDevice(), this->m_vulkanVK.m_bufferVK.getUniformBuffer().bufferMemory);

        this->m_vulkanVK.drawFrame();
    }
}

void Game::shutdown() 
{
    this->m_vulkanVK.destroy();
}