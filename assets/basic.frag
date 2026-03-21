#version 150
uniform sampler2D tex;

in vec2 fragUV;

out vec4 outputColor;

void main() {
    outputColor = vec4(fragUV.y,fragUV.x,0,1);//vec4(texture(tex, fragUV).rgb, 1.0);
}