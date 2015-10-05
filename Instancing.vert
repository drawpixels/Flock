#version 400

layout(location=0) in vec3 vVertex;
layout(location=1) in vec3 vPosition;
layout(location=2) in float vOrientation;
layout(location=3) in vec3 vColor;

out vec3 color;

void main () {
	mat3 transform = mat3 (
		cos(vOrientation), sin(vOrientation), 0.0f, 
		-sin(vOrientation), cos(vOrientation), 0.0f,
		0.0f, 0.0f, 1.0f );
	gl_Position = vec4 ((transform * vVertex)+vPosition, 1.0f);
	color = vColor;
}
