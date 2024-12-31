#pragma once
#include "VulkanCreator.h"
#include "VulkanVK.h"

class Game 
{
	GameInfo m_gameInfo{};
	core::graphic::vulkan::VulkanVK m_vulkanVK;

	/**
	* The loop of the game, which is running constantly while running the game.
	* If have to say to myself. Do watch out to not put this full of useless stuff that drops performance.
	*/
	void gameLoop();

	/**
	* After the game stops this method will be called, which clear every allocated memory.
	*/
	void shutdown();

public:
	
	static GameInfo* instance;

	/**
	* This starts the game and creates everything.
	* 
	* @param gameInfo the information container about the game, that contains the title, or version.
	*/
	explicit Game(const GameInfo& gameInfo);

	/**
	* Starts the game.
	*/
	void start();
};



