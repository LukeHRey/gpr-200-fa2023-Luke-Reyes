//fragmentShader.frag
#version 450
out vec4 FragColor;
in vec2 UV;
//General varriables
uniform float iTime;
uniform float speed;
//Varriables for sky
uniform vec3 skyCol;
uniform vec3 altSkyCol;
//Varriables for sun
uniform vec3 sunCol;
uniform float circleRadius;
//Varriables for buildings
uniform vec3 buildingCol;

float circleSDF(vec2 p, float r){
    return length(p)-r;
}
void main(){
    //Sky
    vec3 col = mix(skyCol, altSkyCol, cos(iTime * speed) - UV.y);
    
    //Sun
    vec2 circlePos = vec2(0.0, (cos(iTime * speed) - circleRadius) - 1.0);
    float d = circleSDF(UV - circlePos,circleRadius);
    d = 1.0 - smoothstep(-circleRadius / 2.0, 0.00, d);
    float sun = d;
    col = mix(col, sunCol, sun);
    
    //Buildings
    float building = step(abs(mod(UV.x*4.0,2.0)-abs(sin(UV.x))), 0.6);
    building = step(UV.y + (abs(sin(UV.x*3.0)/2.0)) + 1.4, building);
    //0.7 for some transparency
    col = mix(col, buildingCol, building * 0.7);
    

    // Output to screen
    FragColor = vec4(col,1.0);
}

