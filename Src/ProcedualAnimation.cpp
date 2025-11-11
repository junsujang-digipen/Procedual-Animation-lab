#include "ProcedualAnimation.h"
#include "Segment.h"
#define TO_FLOAT(x) static_cast<float>(x)
#define TO_INT(x) static_cast<int>(x)

// TODO: compute segment angle for angle restriction
void ForwardReaching(std::vector<Segment>& segments, sf::Vector2f targetPosition, const float distance)
{
	segments.back().setPosition(targetPosition);
	for (int i = segments.size() - 2;i >= 0 ; --i) {
		segments[i].setPosition(GetDistanceConstraint(segments[i+1].getPosition(), segments[i].getPosition(), distance));
	}
}

void BackwardReaching(std::vector<Segment>& segments, sf::Vector2f targetPosition, const float distance)
{
	segments.front().setPosition(targetPosition);
	for (int i = 1;i < segments.size(); ++i) {
		segments[i].setPosition(GetDistanceConstraint(segments[i-1].getPosition(), segments[i].getPosition(), distance));
	}
}

void FABRIK(const sf::Vector2f targetPoint, std::vector<sf::Vector2f>& points, const std::vector<float>& distances, const int maxIteration, const float availableDist)
{
	using Vec2 = sf::Vector2f;
	Vec2 stPoint = points.front();
	for (int i = 0; i < maxIteration; ++i) {
		// Forward reaching
		points.back() = targetPoint;
		for (int j = TO_INT(points.size()) - 2; j >= 0; --j) {
			DistanceConstraint(points[j + 1], points[j], distances[j]);
		}
		// Backward reaching
		points.front() = stPoint;
		for (int j = 1; j < points.size(); ++j) {
			DistanceConstraint(points[j - 1], points[j], distances[j - 1]);
		}
		// end check
		if ((points.back() - targetPoint).length() <= availableDist) {
			break;
		}
	}
}
