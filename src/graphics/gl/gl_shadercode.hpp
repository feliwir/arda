#pragma once
#include <string>

namespace arda::shader
{
	
const std::string sprite_vert =
R"(#version 330

layout(location = 0) in vec2 pos;

void main()
{
	gl_Position = vec4(pos,0.0,1.0);
})";


const std::string sprite_frag =
R"(#version 330

out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0,0.0,0.0,1.0);
})";

}