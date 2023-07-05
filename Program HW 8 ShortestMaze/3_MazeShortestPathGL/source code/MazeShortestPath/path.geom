#version 460 core
layout (lines) in;
layout (points, max_vertices = 1) out;

uniform float t;

void main() {    
    gl_Position = gl_in[0].gl_Position*(t) + gl_in[1].gl_Position*(1.0f-t); 
    gl_PointSize = 10.0f;
    EmitVertex();

    EndPrimitive();
}   