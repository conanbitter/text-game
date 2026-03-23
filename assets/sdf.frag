#version 150
uniform sampler2D tex;
uniform float range;

in vec2 fragUV;
in vec4 color;
in float scale;
in float thickness;
in float roundness;

out vec4 outputColor;

const float aaw = 1.5; // anti-aliasing width

float median(vec3 rgb) {
    return max(min(rgb.r, rgb.g), min(max(rgb.r, rgb.g), rgb.b));
}

void main() {
    float k = 1.0 / (range * scale);

    float msdf = median(texture(tex, fragUV).rgb);
    float sdf = min(msdf, texture(tex, fragUV).a); // min - to remove inner corner bleeding

    float dist = (sdf*roundness + msdf*(1-roundness)) * 2.0 - 1.0;
    dist = dist + 2 * thickness * k;
    float hlim = aaw * k;
    float a = smoothstep(-hlim, hlim, dist);
    /*if(dist > hlim){
        dist = 1.0;
    } else if (dist < -hlim) {
        dist = 0.0;
    } else {
        dist = 0.5;
    }*/
    outputColor = vec4(color.rgb, color.a * a);
}