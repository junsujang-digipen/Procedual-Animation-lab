#include "ProcedualAnimation.h"

#define TO_FLOAT(x) static_cast<float>(x)
#define TO_INT(x) static_cast<int>(x)

void FABRIK(const sf::Vector2f targetPoint, std::vector<sf::Vector2f>& points, const std::vector<float>& distances, const int maxIteration, const float availableDist)
{
	using Vec2 = sf::Vector2f;
	Vec2 stPoint = points.front();
	for (int i = 0; i < maxIteration; ++i) {
		// Forward reaching
		points.back() = targetPoint;
		for (int j = TO_INT(points.size()) - 2; j >= 0; --j) {
			auto dir = points[j] - points[j + 1];
			points[j] = DistanceConstraint(points[j + 1], dir, distances[j]);
		}
		// Backward reaching
		points.front() = stPoint;
		for (int j = 1; j < points.size(); ++j) {
			auto dir = points[j] - points[j - 1];
			points[j] = DistanceConstraint(points[j - 1], dir, distances[j - 1]);
		}
		// end check
		if ((points.back() - targetPoint).length() <= availableDist) {
			break;
		}
	}
}
