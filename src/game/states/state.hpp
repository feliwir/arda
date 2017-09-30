#pragma once
#include <memory>
#include <vector>
#include <string>

namespace arda
{
    class State
    {
	public:
		enum Type
		{
			CUTSCENE,
			MAINMENU,
			INGAME
		};

	public:
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual bool IsFinished() = 0;
    };
}