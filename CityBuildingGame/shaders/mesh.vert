#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aAlternativeTexture; // 0 = normal texture, 1.0 = secondary texture, etc.

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
out float AlternativeTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
    //Normal = normalize(mat3(transpose(inverse(model))) * aNormal);  
    Normal = normalize((model * vec4(aNormal, 0.0)).xyz);
    TexCoords = aTexCoord;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    AlternativeTexture = aAlternativeTexture;

    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}