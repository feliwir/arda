#pragma once
#include <string>

namespace arda::shader
{
	
const std::string sprite_vert =
R"(#version 330
layout(location = 0) in vec2 pos;

out vec2 fragPos;

void main()
{
	fragPos = pos;
	gl_Position = vec4(pos,0.0,1.0);
})";


const std::string sprite_frag =
R"(#version 330
in vec2 fragPos;
out vec4 fragColor;
uniform sampler2D sprite_tex;

void main()
{
	vec2 coords = (fragPos + vec2(1,1))/2.0;
	fragColor = texture(sprite_tex, vec2(coords.x, 1 - coords.y));
})";

}