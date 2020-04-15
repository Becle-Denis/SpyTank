#pragma once
#include <SFML/Graphics.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include "Target.h"
#include "SoundManager.h"
#include "MovingSound.h"
#include "ScreenSize.h"
#include "ResourcesManager.h"

/// <summary>
/// @brief Projectile class 
/// 
/// Manage projectile displacement 
/// </summary>
class Projectile
{
private: //Attributes 
	//sprite of the projectile 
	sf::Sprite m_projectileSprite;

	//Speed
	static double s_projectile_default_speed;
	double m_projectile_speed;

	// The rotation of the projectile.
	double m_rotation{ 0.0 };

	//sound stuffs 
	SoundManager* m_soundManagerPtr{ nullptr };
	MovingSound* m_soundPtr{ nullptr };

public: //functions 

	Projectile();

	/// <summary>
	/// Set the Sprite of the Projectile
	/// </summary>
	/// <param name="texturePtr">Texture of the sprite</param>
	void setProjectile(SoundManager* soundManager = nullptr);

	/// <summary>
	/// Launch the projectile, set the position and the orientation
	/// </summary>
	/// <param name="position">New Position</param>
	/// <param name="rotation">New rotation</param>
	void launch(sf::Vector2f position, double rotation, double speed = s_projectile_default_speed);

	/// <summary>
	/// Return the actual sprite of the projectile 
	/// </summary>
	sf::Sprite const& getSprite() const;

	/// <summary>
	/// Set the Projectile inactive 
	/// </summary>
	void setInactive();

	/// <summary>
	/// Check if the projectile is inactive 
	/// </summary>
	/// <returns></returns>
	bool isInactive() const;

	//update the displacement of the projectile 
	void update(double dt);

	//check for collision on wall, or projectile outside the map
	// Return : 
	//			-1 - No Collision 
	//			1 - Collision with wall 
	//			2 - Outside the map 
	//			3 - Hit the AI Tank DISABLED
	int lifeState(std::map<int, std::list< sf::Sprite> >& wallMap) const;

	bool collideWithSprites(std::pair <sf::Sprite, sf::Sprite> sprites);

	//draw the projectile
	void render(sf::RenderWindow& window ) const;

};



