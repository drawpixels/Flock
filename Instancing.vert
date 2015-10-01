#version 400

layout(location=0) in vec3 vVertex;
layout(location=1) in vec3 vColor;
layout(location=2) in vec3 vPosition;
out vec3 color;

void main () {
	gl_Position = vec4 (vVertex+vPosition, 1.0);
	color = vColor;
}
