#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in float Brightness;

uniform sampler2D texture1;

void main()
{
    vec4 color = texture(texture1, TexCoord) * Brightness;
    if (color.a == 0) {
        discard;
    }
    FragColor = texture(texture1, TexCoord) * Brightness;
}