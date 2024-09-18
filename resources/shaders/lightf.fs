#version 330
uniform sampler2D tex0;

in vec3 color;
in vec2 texCoord;
out vec4 fin_color;
uniform int CC;
void main(void) {
	if(CC == 1)
		fin_color = texture(tex0, texCoord);
	else if(CC==0)
		fin_color = vec4(color, 2.0);
	else if(CC==2)
		fin_color = vec4(1.0f,1.0f,1.0f, 3.0);
}