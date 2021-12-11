#include "Flock.h"
#include <iostream>
#include "Boid.h"

Flock::Flock(int group_size)
{
	for (int i = 0; i < group_size; ++i) {
		boids.push_back(Boid());
	}
}

std::vector<float> Flock::get_position_2d_normalized() const
{
	std::vector<float> res;
	for (auto& boid : boids) {
		res.push_back(boid.position.x() / Boid::width * 2 - 1);
		res.push_back((Boid::height - boid.position.y()) / Boid::height * 2 - 1);
	}
	return res;
}

std::vector<float> Flock::update(Boid::Flag mode)
{
	for (auto& boid : boids) {
		boid.update(boids,mode);
	}
	return get_position_2d_normalized();
}
