#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

uniform sampler2D texture;
in vec4 textureCoord;
in int isHighlighted;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;


void main()
{
	vec2 latLong = vec2((atan(textureCoord.y, textureCoord.x) / 3.1415926 + 1.0) * 0.5,
						(asin(textureCoord.z) / 3.1415926 + 0.5));
	
    fragColor = texture2D(texture, latLong);
}
