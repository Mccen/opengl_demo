#version 430
uniform sampler2D tex0;

in vec3 color;
in vec2 texCoord;
out vec4 fin_color;

uniform uint hasTex;
void main(void) {
	if(hasTex == 1)
		//fin_color = texture(tex0, texCoord);
		fin_color = mix(texture(tex0, texCoord),vec4(color,1.0f),0.6);
	else
		fin_color = vec4(color, 2.0);

}