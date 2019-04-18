#version 330 core
layout (location = 0) in vec3 position;// <vec3 pos>
layout (location = 1) in vec2 texture;// <vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(position.xyz, 1.0);
    TexCoords = texture;
}
