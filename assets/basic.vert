#version 150

const vec2 verts[6] = vec2[6](
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0),
    vec2(-1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0, -1.0)
);

const vec2 uvs[6] = vec2[6](
    vec2(0.0, 1.0),
    vec2(1.0, 1.0),
    vec2(0.0, 0.0),
    vec2(1.0, 1.0),
    vec2(1.0, 0.0),
    vec2(0.0, 0.0)
);


uniform vec4 scale;

out vec2 fragUV;

void main() {
    vec2 pos = verts[gl_VertexID % 6];
    gl_Position = vec4(pos*0.5, 0.0, 1);//vec4(vert.x*scale.x+scale.z, vert.y*scale.y+scale.w, 0.0, 1.0);
    fragUV = uvs[gl_VertexID % 6];
}