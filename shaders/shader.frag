#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in float Brightness;

uniform sampler2D texture1;

void main()
{
    // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    FragColor = texture(texture1, TexCoord) * Brightness;
}