#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;
out vec3 ourColor;
out vec2 texCoord;
uniform float mPosx;
uniform float mPosy;
void main()
{
	gl_Position = vec4(aPos+vec3(mPosx,mPosy,0.0f), 1.0);
	ourColor = aCol;
	texCoord = aTex;
}