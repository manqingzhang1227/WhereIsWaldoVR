#version 330 core
//layout (location = 0) in vec3 position;// <vec3 pos>
//layout (location = 1) in vec2 texture;// <vec2 tex>
layout (location = 0) in vec4 vertex;// <vec3 pos>

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    //gl_Position = projection * vec4(position.xyz, 1.0);
    //TexCoords = texture;

	gl_Position = projection * vec4(vertex.xy, 0.0 , 1.0);
    TexCoords = vertex.zw;
}
