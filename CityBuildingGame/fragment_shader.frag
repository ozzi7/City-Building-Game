#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);

	//FragColor = vec4(1.0, 1.0, 1.0, 1.0); //Set colour of each fragment to WHITE
}