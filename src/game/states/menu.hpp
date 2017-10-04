#pragma once
#include "state.hpp"

namespace arda
{
	class GUI;

    class Menu final : public State
    {
    public:
        Menu(GUI& gui);

        // Geerbt �ber State
		virtual void Start() override;
		virtual void Update() override;
		virtual bool IsFinished() override;
	private:
		GUI& m_gui;
    };
}