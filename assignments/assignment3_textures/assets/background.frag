#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _SupermarketTexture;
layout(location = 1) uniform sampler2D _NoiseTexture;

uniform float iTime;

void main(){
    vec4 t0 = texture2D(_SupermarketTexture, UV);
    vec4 t1 = texture2D(_NoiseTexture, UV + sin(iTime)/2.0);

	FragColor = mix(t1, t0, 0.8f);
}
