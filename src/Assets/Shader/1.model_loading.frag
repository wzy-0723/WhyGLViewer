#version 450 core
out vec4 FragColor;
in vec3 vColor;

uniform vec4 uColor;

void main()
{    
    FragColor = vec4(vColor, 1.0f);
    //FragColor = vec4(vColor, 1.0f) * uColor;
}