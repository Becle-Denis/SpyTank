#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include "SoundManager.h"
#include "GameState.h"

/// <summary>
/// A class to store all the Projectiles 
/// Follow the pool design pattern 
/// </summary>
class ProjectilePool
{
private: //Attributes 
	// vector of the projectiles 
	std::vector<std::shared_ptr<Projectile>> m_projectiles;

	// Actual index of the next Projectile that should be available to use 
	unsigned int m_projectileIndex;

public: //functions 
	/// <summary>
	/// Create all the Projectiles ans set them inactive,
	/// This is the only one function that create Projectiles  
	/// </summary>
	/// <param name="texture">Texture of the projectiles</param>
	/// <param name="poolSize">Maximum number of projetiles that can be used at the same time</param>
	ProjectilePool(unsigned int poolSize, SoundManager& soundManager);

	/// <summary>
	/// delete all the projectiles 
	/// </summary>
	~ProjectilePool();

	/// <summary>
	/// Check for the next projectile available and return a pointer to it
	/// </summary>
	/// <returns>A pointer to the next available projectile, nullptr if there is no more available</returns>
	std::shared_ptr<Projectile> getProjectile();

	/// <summary>
	/// Set the Light Mode to all Projectiles 
	/// </summary>
	void setLightMode(LightMode mode);
};

