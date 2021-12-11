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

	// 获得能在opengl中显示的数据
	std::vector<float> get_position_2d_normalized() const;

	// 群体完成一次运动
	std::vector<float> update(Boid::Flag mode);
};

