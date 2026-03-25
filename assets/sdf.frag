#version 150
uniform sampler2D tex;
uniform float range;
uniform float windowScale;

in vec2 fragUV;
in vec4 color;
in float scale;
in float thickness;
in float roundness;
in float blur;

out vec4 outputColor;

const float aaw = 1.5; // anti-aliasing width
const float MAX_OUT_LIMIT = 0.1 * 2.0 / 255.0 - 1.0;
const float OUTER_EDGE = 1.0; //- 1.0e-5; maybe add bias

float median(vec3 rgb) {
    return max(min(rgb.r, rgb.g), min(max(rgb.r, rgb.g), rgb.b));
}

void main() {
    float k = 1.0 / (range * scale);

    float msdf = median(texture(tex, fragUV).rgb);
    float sdf = min(msdf, texture(tex, fragUV).a); // min - to remove inner corner bleeding

    float dist = (sdf*roundness + msdf*(1-roundness)) * 2.0 - 1.0;
    
    float aa_limit = aaw * k / windowScale;    

    float shape_offset = min(2 * thickness * k, OUTER_EDGE - aa_limit);
    dist += shape_offset;

    float blur_limit = -blur * 2 * k;

    float inner_limit = aa_limit;
    float outer_limit = clamp(blur_limit, -OUTER_EDGE + shape_offset, -aa_limit);

    float a = smoothstep(outer_limit, inner_limit, dist);
    outputColor = vec4(color.rgb, color.a * a);
    /*float a = dist;
    if(a > inner_limit){
        a = 1.0;
    } else if (a < outer_limit) {
        a = 0.0;
    } else {
        a = 0.5;
    }
    outputColor = vec4(vec3(a), 1.0);*/
}