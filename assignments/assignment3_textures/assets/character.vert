#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;
uniform float iTime;
void main(){
	UV = vUV;
	vec4 pos = vec4(vPos,1.0);
	gl_Position = pos + vec4((sin(iTime))/2.0, -(cos(iTime))/3.0, 0.0, 0.0);
}