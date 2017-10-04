#pragma once
#include <vector>
#include <memory>
#include <chrono>

namespace arda
{
	class Sprite;

	//Blend a set of sprites over another
    class BlendAnimation
    {
	public:
	struct Keyframe
	{
		int Time;
		float Opacity;
	};

	public:
		BlendAnimation(std::vector<Keyframe> keyframes, std::vector<std::shared_ptr<Sprite>> first,
			std::vector<std::shared_ptr<Sprite>> second);

		void Start();

		void Update();
	private:
		std::chrono::high_resolution_clock::time_point m_start;
		std::vector<Keyframe> m_keyframes;
		std::vector<std::shared_ptr<Sprite>> m_first;
		std::vector<std::shared_ptr<Sprite>> m_second;
    };
}