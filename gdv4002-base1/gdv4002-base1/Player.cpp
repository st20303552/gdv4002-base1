#include "Engine.h"
#include "Player.h"
#include "Keys.h"
#include <bitset>
#include <complex>

using std::complex;
extern std::bitset<5> keys;
extern glm::vec2 gravity;

Player::Player(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID, float mass) : GameObject2D(initPosition, initOrientation, initSize, initTextureID)
{
	this->mass = mass;

	velocity = glm::vec2(0.0f, 0.0f);
}

void Player::update(double tdelta)
{
	complex<float> i = complex<float>(0.0f, 1.0f);
	complex<float> dir = exp(i * GameObject2D::orientation);
	glm::vec2 F = glm::vec2(0.0f, 0.0f);
	const float thrust = 1.5f;
	const float playerTurnSpeed = 2.0f;

	// 1) Accumulate forces
	if (keys.test(Key::W) == true)
	{
		F += glm::vec2(dir.real(), dir.imag()) * thrust;
	}
	if (keys.test(Key::S) == true)
	{
		F -= glm::vec2(dir.real(), dir.imag()) * thrust;
	}
	if (keys.test(Key::A) == true)
	{
		GameObject2D::orientation += (playerTurnSpeed * float(tdelta));
	}
	if (keys.test(Key::D) == true)
	{
		GameObject2D::orientation -= (playerTurnSpeed * float(tdelta));
	}

	F += gravity;

	// Add impulse force
	if (position.y < -getViewplaneHeight() / 2.0f)
	{
		F += glm::vec2(0.0f, 10.0f);
	}

	if (position.y > getViewplaneHeight() / 2.0f)
	{
		F -= glm::vec2(0.0f, 10.0f);
	}
	if (position.x < -getViewplaneWidth() / 2.0f)
	{
		F += glm::vec2(10.0f, 0.0f);
	}

	if (position.x > getViewplaneWidth() / 2.0f)
	{
		F -= glm::vec2(10.0f, 0.0f);
	}

	// 2) Calculate acceleration. If f = m*a, a = f/m.
	glm::vec2 a = F * (1.0f / mass);

	// 3) Integrate to get new velocity.
	velocity = velocity + (a * (float)tdelta);

	// 4) Integrate to get new position.
	position = position + (velocity * (float)tdelta);
}