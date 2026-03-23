#version 150
uniform sampler2D tex;

in vec2 fragUV;
in float scale;

out vec4 outputColor;

const float w = 1.5;
const float pxr = 32.0;
const float offset = 0.0;

void main() {
    float dist = texture(tex, fragUV).r * 2.0 - 1.0;
    dist = dist + 2 * offset / (pxr * scale);
    float hlim = w / (pxr * scale);
    dist = smoothstep(-hlim, hlim, dist);
    /*if(dist > hlim){
        dist = 1.0;
    } else if (dist < -hlim) {
        dist = 0.0;
    } else {
        dist = 0.5;
    }*/
    outputColor = vec4(vec3(dist), 1.0);
}