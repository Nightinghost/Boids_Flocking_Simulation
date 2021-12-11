#pragma once

#include <iostream>
#include "Boid.h"
#include <vector>

class Flock
{
private:
	std::vector<Boid> boids;

public:
	Flock(int group_size);

	// �������opengl����ʾ������
	std::vector<float> get_position_2d_normalized() const;

	// Ⱥ�����һ���˶�
	std::vector<float> update(Boid::Flag mode);
};

