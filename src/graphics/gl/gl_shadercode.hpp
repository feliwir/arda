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

uniform sampler2D color_tex;
uniform sampler2D mask_tex;
uniform float opacity;
uniform bool use_mask;

out vec4 fragColor;

void main()
{
	vec2 coords = fragCoord;
	fragColor = texture(color_tex, vec2(coords.x, 1 - coords.y));
	if(use_mask)
	{
		fragColor.a = texture(mask_tex, vec2(coords.x, 1 - coords.y)).r;
	}
	fragColor.a *= opacity;
	
	if(fragColor.a<0.01)
		discard;
})";

}