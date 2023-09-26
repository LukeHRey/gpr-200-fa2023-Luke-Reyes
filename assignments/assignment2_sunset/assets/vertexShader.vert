#version 450
layout(location = 0) in vec3 vPos;
out vec2 UV;
void main(){
	UV = vec2(vPos[0]/0.8, vPos[1]/0.8);
	gl_Position = vec4(vPos,1.0);
}