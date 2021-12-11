#pragma once

#include <Eigen/Eigen>
#include <ctime>
#include <iostream>
#include <vector>

class Boid
{
public:
	Eigen::Vector2f position;
	Eigen::Vector2f velocity;
	Eigen::Vector2f acceleration;

	static float maxForce;		// 最大受力
	static float maxSpeed;		// 最大速度

	static float width;			// 窗口范围
	static float height;

	static float thread_num;	// 线程数量
	
	//	控制模式
	enum Flag { NOCONTROL, ALIGNMENT, COHESION, SEPARATION, FLOCKING }mode;

public:
	Boid() {
		position = Eigen::Vector2f(rand() % 800, rand() % 600);
		velocity = Eigen::Vector2f::Random() * (rand() % 2 + 2);
		acceleration = Eigen::Vector2f::Zero();
	}
	// copy-constructor
	Boid(const Boid& b) {
		this->position = b.position;
		this->velocity = b.velocity;
		this->acceleration = b.acceleration;
	}

	// 更新一次位置
	void update(std::vector<Boid>& boids, Flag mode);

	// 设置控制模式
	void set_flocking_mode(Flag f);

private:
	// 边界检查
	void boundary();

	Eigen::Vector2f align(std::vector<Boid>& boids);
	Eigen::Vector2f cohesion(std::vector<Boid>& boids);
	Eigen::Vector2f separation(std::vector<Boid>& boids);
	
	// 求一次运动中的受力（加速度）
	void flock(std::vector<Boid>& boids, Flag mode);
};

