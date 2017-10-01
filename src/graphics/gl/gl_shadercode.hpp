#pragma once
#include <string>

namespace arda::shader
{
	
const std::string sprite_vert =
R"(#version 330
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 coord;
out vec2 fragPos;
out vec2 fragCoord;

void main()
{
	fragPos = pos;
	fragCoord = coord;
	gl_Position = vec4(pos,0.0,1.0);
})";


const std::string sprite_frag =
R"(#version 330
in vec2 fragPos;
in vec2 fragCoord;
out vec4 fragColor;
uniform sampler2D sprite_tex;

void main()
{
	vec2 coords = fragCoord;
	fragColor = texture(sprite_tex, vec2(coords.x, 1 - coords.y));
})";

}