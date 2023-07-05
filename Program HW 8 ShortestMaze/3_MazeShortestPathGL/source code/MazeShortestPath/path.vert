# version 460 core

layout(location = 0) in vec3 in_position;
uniform mat4 model;
void main(){
	//gl_Position = vec4(in_position * 0.1, 1.0f);
	vec3 pos = vec3(model * vec4(in_position, 1.0f));
	gl_Position = vec4(pos, 1.0f);
}