#include "MotionCalculator.h"

static Vector3 prevCoords{ 0, 0, 0 };
static float prevTime = 0;
struct Coords {
	Vector3 pos;
	float time;
	//uintptr_t id;
};
static std::queue<Coords> coordsQueue;

//计算速度向量
Vector3 calculate_enemy_velocity(Vector3 currCoords) {  

	float t_diff = 0.5f;     //测速间隔大小
	Vector3 enemy_velocity;

	float currTime = clock();

	if (prevCoords[0] == 0 && prevTime == 0) {
		prevTime = currTime;
		//第一次初始化时间
	}

	else {
		enemy_velocity = (currCoords - prevCoords) / t_diff;// Calculate velocity vector of enemy

	}
	if (prevCoords[0] == 0) {
		prevCoords = currCoords;        //第一次初始化之前坐标

	}
	// 将当前的currCoords值添加到队列的末尾
	Coords curr;
	curr.pos = currCoords;
	curr.time = clock();
	coordsQueue.push(curr);
	// 删除队列中超过测速间隔秒钟的值
	while (!coordsQueue.empty() && (clock() - coordsQueue.front().time) / CLOCKS_PER_SEC >= t_diff) {
		coordsQueue.pop();
	}
	// 将prevCoords设置为队列中最早的值
	if (!coordsQueue.empty()) {
		Coords prev = coordsQueue.front();
		prevCoords = prev.pos;

	}
	return enemy_velocity;
}






//计算领先位置
Vector3 calculate_aiming_point(Vector3 currCoords, Vector3 enemy_velocity, float time_to_impact) {

	//float  time_to_impact = unitproj.distance / unitproj.shellVelocity ;// 计算子弹飞行时间; 不准，此法只在抛射物匀速直线运动时有用

	Vector3 predicted_position;


	//  Calculate  predicted position of enemy
	predicted_position = currCoords + enemy_velocity * time_to_impact;

	return predicted_position;

}