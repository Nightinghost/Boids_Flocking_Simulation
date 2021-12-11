#include "Boid.h"
#include <omp.h>

float Boid::maxForce = 0.2;
float Boid::maxSpeed = 2;

float Boid::width = 800;
float Boid::height = 600;

float Boid::thread_num = 8;

void Boid::boundary()
{
	if (position.x() > width)
		position.x() = 0;
	else if (position.x() < 0)
		position.x() = width;

	if (position.y() > height)
		position.y() = 0;
	else if (position.y() < 0)
		position.y() = height;
}

Eigen::Vector2f Boid::align(std::vector<Boid>& boids)
{
	Eigen::Vector2f steering = Eigen::Vector2f::Zero();
	int total = 0;
	float perceptionRadius = 25;	// 感知半径

	omp_set_num_threads(thread_num);

#pragma omp parallel
	{
#pragma omp parallel for
		for (int i = 0; i < boids.size();++i) {
			if (&boids[i] != this)
			{
				float dis = (this->position - boids[i].position).norm();
				if (dis < perceptionRadius) {
#pragma omp critical
					steering += boids[i].velocity;
#pragma omp critical
					total++;
				}
			}
		}
	}
	
	if (total > 0) {
		steering = steering / total;
		steering *= maxSpeed;
		steering = steering - this->velocity;

		// 限制最大受力
		if (steering.norm() > maxForce) {
			steering.normalize();
			steering *= maxForce;
		}
	}
	return steering;
}

Eigen::Vector2f Boid::cohesion(std::vector<Boid>& boids)
{
	Eigen::Vector2f steering = Eigen::Vector2f::Zero();
	int total = 0;
	float perceptionRadius = 100;

	omp_set_num_threads(thread_num);
#pragma omp parallel
	{
#pragma omp parallel for
		for (int i = 0; i < boids.size();++i) {
			if (&boids[i] != this)
			{
				float dis = (this->position - boids[i].position).norm();
				if (dis < perceptionRadius) {
#pragma omp critical
					steering += boids[i].position;
#pragma omp critical
					total++;
				}
			}
		}
	}

	if (total > 0) {
		steering = steering / total;
		steering = steering - this->position;
		steering.normalize();
		steering *= maxSpeed;
		steering -= this->velocity;

		// 限制最大受力
		if (steering.norm() > maxForce) {
			steering.normalize();
			steering *= maxForce;
		}
	}
	return steering;
}

Eigen::Vector2f Boid::separation(std::vector<Boid>& boids)
{
	Eigen::Vector2f steering = Eigen::Vector2f::Zero();
	int total = 0;
	float perceptionRadius = 50;

	omp_set_num_threads(thread_num);
#pragma omp parallel
	{
#pragma omp parallel for
		for (int i = 0; i < boids.size(); ++i) {
			if (&boids[i] != this)
			{
				float dis = (this->position - boids[i].position).norm();
				if (dis < perceptionRadius) {
					Eigen::Vector2f direction = this->position - boids[i].position;
					direction = direction / (dis * dis);
#pragma omp critical
					steering += direction;
#pragma omp critical
					total++;
				}
			}
		}
	}

	if (total > 0) {
		steering = steering / total;

		steering.normalize();
		steering *= maxSpeed;

		steering -= this->velocity;

		// limit the max force
		if (steering.norm() > maxForce) {
			steering.normalize();
			steering *= maxForce;
		}
	}
	return steering;
}

void Boid::flock(std::vector<Boid>& boids, Flag mode)
{
	Eigen::Vector2f alignment, cohe, sepa;
	switch (mode)
	{
	case Boid::ALIGNMENT:
		alignment = align(boids);
		this->acceleration += alignment;
		break;
	case Boid::COHESION:
		cohe = cohesion(boids);
		this->acceleration += cohe;
		break;
	case Boid::SEPARATION:
		sepa = separation(boids);
		this->acceleration += sepa;
		break;
	case Boid::FLOCKING:
		alignment = align(boids);
		cohe = cohesion(boids);	
		sepa = separation(boids);
		this->acceleration += alignment;
		this->acceleration += cohe;
		this->acceleration += sepa;
		break;
	default:
		break;
	}
}

void Boid::update(std::vector<Boid>& boids, Flag mode)
{
	boundary();
	flock(boids, mode);

	this->position += this->velocity;
	this->velocity += this->acceleration;

	// limit the max velocitye
	if (this->velocity.norm() > maxSpeed) {
		this->velocity.normalize();
		this->velocity *= maxSpeed;
	}

	this->acceleration *= 0;
}

void Boid::set_flocking_mode(Flag f)
{
	this->mode = f;
}
