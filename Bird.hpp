//---- 
// Bird.hpp
//----

#ifndef BIRD_HPP
#define BIRD_HPP

#include <glm/glm.hpp>

class Bird {
public:
	Bird ();
	~Bird ();
	void SetPosition (glm::vec2 p);
	glm::vec2 GetPosition ();
	void SetBearing (float o);
	float GetBearing ();
	void SetSpeed (float s);
	float GetSpeed ();
	glm::vec2 GetVelocity ();
	glm::vec3 GetColor();
	void Move (float dt);
	void Align (Bird* b, int num, int self);
	
private:
	glm::vec2 m_Position;
	float m_Bearing;
	float m_Speed;
	glm::vec3 m_Color;
};

#endif
