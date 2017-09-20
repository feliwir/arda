#pragma once
#include "../../core/debugger.hpp"
#include <string_view>
#include <d3d11.h>

namespace arda
{
    namespace d3d
    {
		void checkError(HRESULT code, std::string_view msg)
		{
			if (code != S_OK)
			{
				ARDA_LOG(msg);
			}
		}
    }
}