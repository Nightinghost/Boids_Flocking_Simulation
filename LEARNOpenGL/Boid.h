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

	static float maxForce;		// �������
	static float maxSpeed;		// ����ٶ�

	static float width;			// ���ڷ�Χ
	static float height;

	static float thread_num;	// �߳�����
	
	//	����ģʽ
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

	// ����һ��λ��
	void update(std::vector<Boid>& boids, Flag mode);

	// ���ÿ���ģʽ
	void set_flocking_mode(Flag f);

private:
	// �߽���
	void boundary();

	Eigen::Vector2f align(std::vector<Boid>& boids);
	Eigen::Vector2f cohesion(std::vector<Boid>& boids);
	Eigen::Vector2f separation(std::vector<Boid>& boids);
	
	// ��һ���˶��е����������ٶȣ�
	void flock(std::vector<Boid>& boids, Flag mode);
};

