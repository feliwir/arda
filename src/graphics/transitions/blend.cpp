#include "blend.hpp"
#include "../sprite.hpp"
#include <algorithm>
#include <glm/glm.hpp>

arda::BlendAnimation::BlendAnimation(std::vector<Keyframe> keyframes, 
	std::vector<std::shared_ptr<Sprite>> first, std::vector<std::shared_ptr<Sprite>> second) :
	m_first(first),m_second(second),m_keyframes(keyframes), m_finished(false)
{
	//sort the keyframes to their time
	std::sort(m_keyframes.begin(), m_keyframes.end(), 
		[](const Keyframe & a, const Keyframe & b) -> bool
	{
		return a.Time < b.Time;
	});
}

void arda::BlendAnimation::Start()
{
	m_start = std::chrono::high_resolution_clock::now();
}

void arda::BlendAnimation::Update()
{
	//current time
	auto now = std::chrono::high_resolution_clock::now();
	auto passed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start).count();

	//get the both keyframes where we sitting

	auto first = m_keyframes.front();
	auto second = m_keyframes.back();

	if (passed > 1000)
	{
		int c = 0;
	}

	int i = 0;
	for (auto key : m_keyframes)
	{
		if (key.Time > passed)
		{
			second = key;
			break;
		}

		first = key;
	}

	if (first.Time == second.Time)
	{
		m_finished = true;
		return;
	}
		

	//get how much both keys contribute to the alpha:
	double span = second.Time - first.Time;
	float contrib = (passed - first.Time) / span;

	//get the opacity for group a
	float opacity = glm::mix(first.Opacity, second.Opacity, contrib);

	for (auto& a : m_first)
	{
		a->SetOpacity(1-opacity);
	}

	for (auto& b : m_second)
	{
		b->SetOpacity(opacity);
	}
}

bool arda::BlendAnimation::IsFinished()
{
	return m_finished;
}
