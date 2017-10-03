#pragma once
#include "state.hpp"

namespace arda
{
    class Menu final : public State
    {
    public:
        Menu();

        // Geerbt �ber State
		virtual void Start() override;
		virtual void Update() override;
		virtual bool IsFinished() override;
    };
}