#include "LevelLoader.h"

/// The various operator >> overloads below are non-member functions used to extract
///  the game data from the YAML data structure.

/// <summary>
/// @brief Extracts the obstacle type, position and rotation values.
/// 
/// </summary>
/// <param name="obstacleNode">A YAML node</param>
/// <param name="obstacle">A simple struct to store the obstacle data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& obstacleNode, ObstacleData& obstacle)
{
	obstacle.m_type = obstacleNode["type"].as<std::string>();
	obstacle.m_position.x = obstacleNode["position"]["x"].as<float>();
	obstacle.m_position.y = obstacleNode["position"]["y"].as<float>();
	obstacle.m_rotation = obstacleNode["rotation"].as<double>();
}

/// <summary>
/// @brief Extracts the target position and times values.
/// calculate the random offset 
/// 
/// </summary>
/// <param name="targetNode">A YAML node</param>
/// <param name="target">A simple struct to store the target data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& targetNode, TargetData& target)
{
	float randomOffset = targetNode["position"]["randomOffset"].as<float>();
	float xPos = targetNode["position"]["x"].as<float>();
	xPos = xPos - randomOffset + rand() % ((int) randomOffset);
	float yPos = targetNode["position"]["y"].as<float>();
	yPos = yPos - randomOffset + rand() % ((int)randomOffset);
	target.m_position.x = xPos;
	target.m_position.y = yPos;
	target.secondsToStart = targetNode["secondsToStart"].as<float>();
	target.secondsOnScreen = targetNode["secondsOnScreen"].as<float>();
}

/// <summary>
/// @brief Extracts the initial screen position for the player tank.
/// 
/// </summary>
/// <param name="tankNode">A YAML node</param>
/// <param name="tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& tankNode, TankData& tank)
{
	tank.m_position.x = tankNode["position"]["x"].as<float>();
	tank.m_position.y = tankNode["position"]["y"].as<float>();
}


/// <summary>
/// @brief Top level function that extracts various game data from the YAML data stucture.
/// 
/// Invokes other functions to extract the background, tank and obstacle data.
//   Because there are multiple obstacles, obstacle data are stored in a vector.
/// </summary>
/// <param name="tankNode">A YAML node</param>
/// <param name="tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& levelNode, LevelData& level)
{
	levelNode["tank"] >> level.m_tank;

	levelNode["ai_tank"] >> level.m_aiTank;

	const YAML::Node& obstaclesNode = levelNode["obstacles"].as<YAML::Node>();
	for (unsigned i = 0; i < obstaclesNode.size(); ++i)
	{
		ObstacleData obstacle;
		obstaclesNode[i] >> obstacle;
		level.m_obstacles.push_back(obstacle);
	}

	const YAML::Node& targetNode = levelNode["targets"].as<YAML::Node>();
	for (unsigned i = 0; i < targetNode.size(); ++i)
	{
		TargetData target;
		targetNode[i] >> target;
		level.m_targets.push_back(target);
	}
}

////////////////////////////////////////////////////////////
void LevelLoader::load(int nr, LevelData& level)
{
	std::stringstream ss;
	//added 
	ss << "./resources/levels/";

	ss << "level";
	ss << nr;
	ss << ".yaml";

	try
	{
		YAML::Node baseNode = YAML::LoadFile(ss.str());
		if (baseNode.IsNull())
		{
			std::string message("File: " + ss.str() + " not found");
			throw std::exception(message.c_str());
		}
		baseNode >> level;
	}
	catch (YAML::ParserException& e)
	{
		std::string message(e.what());
		message = "YAML Parser Error: " + message;
		throw std::exception(message.c_str());
	}
	catch (std::exception& e)
	{
		std::string message(e.what());
		message = "Unexpected Error: " + message;
		throw std::exception(message.c_str());
	}
}


