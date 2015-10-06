//----
// Bird.cpp
//----

#include "Bird.hpp"
#include <math.h>
#include <stdio.h>

#define VIS_DIST 0.4f
#define VIS_ANGLE 2.5f
#define MIN_DIST 0.2f
#define MAX_SPEED 0.15f
#define MIN_SPEED 0.05
#define MAX_CHANGE 0.1f
#define COEFF_AVOID 0.0002f
#define COEFF_MATCH 0.001f
#define COEFF_CENTER 0.001f

Bird::Bird () : 
	m_Position (glm::vec2(0.0f,0.0f)), 
	m_Bearing (0.0f), 
	m_Speed (0.0f), 
	m_Color (glm::vec3(0.0f,0.0f,0.0f)) {};

Bird::~Bird () {};

void Bird::SetPosition (glm::vec2 p) {
	m_Position = p;
}

glm::vec2 Bird::GetPosition () {
	return m_Position;
}

void Bird::SetBearing (float o) {
	m_Bearing = o;
}

float Bird::GetBearing () {
	return m_Bearing;
}

void Bird::SetSpeed (float o) {
	m_Speed = o;
}

float Bird::GetSpeed () {
	return m_Speed;
}

glm::vec2 Bird::GetVelocity () {
	return glm::vec2(-m_Speed*sin(m_Bearing),m_Speed*cos(m_Bearing));
}

glm::vec3 Bird::GetColor () {
	return m_Color;
}

void Bird::Move (float dt) {
	glm::vec2 velocity = GetVelocity();
	m_Position = m_Position + velocity * dt;
	if (m_Position.x > 1.0f)
		m_Position.x = m_Position.x - 2.0;
	if (m_Position.x < -1.0f)
		m_Position.x = m_Position.x + 2.0;
	if (m_Position.y > 1.0f)
		m_Position.y = m_Position.y - 2.0;
	if (m_Position.y < -1.0f)
		m_Position.y = m_Position.y + 2.0;
}

void Bird::Align (Bird* b, int num, int self) {
	//-- Check every birds in the flock except one's self
	m_Color = (self==0) ? glm::vec3(0.0f,1.0f,1.0f) : glm::vec3(1.0f,1.0f,1.0f);
	int num_neighbour = 0;
	glm::vec2 vAvoidance = glm::vec2(0.0f,0.0f); 
	glm::vec2 vMatching = glm::vec2(0.0f,0.0f); 
	glm::vec2 vCentering = glm::vec2(0.0f,0.0f);
	for (int i=0; i<num; i++) {
		if (i!=self) {
			glm::vec2 delta = m_Position - b[i].GetPosition();
			float dist = glm::length(delta);
			float angle = std::atan2(-delta.x,delta.y) - m_Bearing - M_PI;
			if (angle>M_PI) angle -= 2*M_PI;
			if (angle<-M_PI) angle += 2*M_PI;
			/*---- DEBUG PRINT ----*
			if (self==0) printf("d=%7.4f(%c) a=%7.4f(%c)  ",dist,(dist<VIS_DIST)?'T':'F',angle,(std::abs(angle)<VIS_ANGLE)?'T':'F');
			*---- DEBUG PRINT ----*/
			//-- Consider the birds within the visible distanes and angle
			if ((dist<VIS_DIST) && (std::abs(angle)<VIS_ANGLE)) {
				m_Color = glm::vec3(1.0f,0.0f,0.0f);
				num_neighbour++;
				float strength;
				//-- Collision avoidance
				if (dist<MIN_DIST) {
					strength = (dist<0.05) ? (COEFF_AVOID/0.05/0.05) : (COEFF_AVOID/dist/dist);
					vAvoidance = vAvoidance + glm::normalize(delta) * strength;
				}
				//-- Velocity Matching
				vMatching = vMatching + b[i].GetVelocity();
				//-- Flock Centering
				
			}
		}
	}
	/*---- DEBUG PRINT ----*
	if (self==0) printf("%6.4f,%6.4f\n",VIS_DIST,VIS_ANGLE);
	*---- DEBUG PRINT ----*/
	vMatching = vMatching / float(num_neighbour) - GetVelocity();
	glm::vec2 vChange = vAvoidance + vMatching + vCentering;
	float len_change = glm::length(vChange);
	if (len_change>0.0f) {
		//-- Limit the change in speed within possible range
		if (len_change>MAX_CHANGE) 
			vChange = glm::normalize(vChange) * MAX_CHANGE;
		glm::vec2 vVelocity = GetVelocity() + vChange;
		m_Speed = glm::length(vVelocity);
		if (m_Speed > MAX_SPEED) m_Speed = MAX_SPEED;
		if (m_Speed < MIN_SPEED) m_Speed = MIN_SPEED;
		m_Bearing = atan2(-vVelocity.x,vVelocity.y);
	}
}


