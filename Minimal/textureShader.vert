#version 410 core
// This is a sample fragment shader.


// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
uniform mat4 projection;
uniform mat4 modelview;

attribute vec3 v_coord;

// You can output many things. The first vec4 type output determines the color of the fragment
varying vec3 textureCoord




void main()
{
	gl_Position = projection * modelview * vec4(v_coord, 1.0);
	textureCoord = vec4(v_coord, 1.0);
}
