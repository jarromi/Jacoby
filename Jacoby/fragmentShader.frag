#version 330 core
in vec3 ourColor;
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float ourFloat;
void main()
{
	FragColor = vec4(ourColor,ourFloat);
	FragColor = mix(texture(ourTexture1,texCoord),texture(ourTexture2,texCoord),0.2)*vec4(ourColor,ourFloat);
	//FragColor = texture(ourTexture1,texCoord)*vec4(ourColor,ourFloat);
}