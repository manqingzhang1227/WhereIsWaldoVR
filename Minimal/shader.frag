#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 vertNormal;

uniform int isHighlighted;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;




void main()
{
	vec3 color = vertNormal;
	//if (!all(equal(color, abs(color)))) {
    //    color = vec3(1.0) - abs(color);
    //}
	color = vec3( 0.8,0.8,0.7);
	
	
	if( isHighlighted == 1 ) {
		color = vec3( 1, 0,0);

	}

	if( isHighlighted == 2) {
		color = vec3(1,1,0);
	}

    fragColor = vec4(color, 1.0);
}
