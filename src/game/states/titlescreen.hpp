#pragma once
#include "state.hpp"

namespace arda
{
	class TitleScreen final : public State
	{
	public:

		// Geerbt über State
		virtual void Start() override;
		virtual void Update() override;
		virtual bool IsFinished() override;
	};
}