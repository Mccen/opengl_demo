#version 430
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 acolor;
layout(location = 2) in vec2 atex;

out vec3 color;
out vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 per;
void main(void){
	vec4 position = vec4(aPos,1.0);
	gl_Position = per*view*model*position;
	color = acolor;
	texCoord = atex;
}