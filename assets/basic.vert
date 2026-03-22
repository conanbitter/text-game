#version 150

const vec2 verts[6] = vec2[6](
    vec2(0.0, 1.0),
    vec2(1.0, 1.0),
    vec2(0.0, 0.0),
    vec2(1.0, 1.0),
    vec2(1.0, 0.0),
    vec2(0.0, 0.0)
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
uniform samplerBuffer spriteData;

out vec2 fragUV;

void main() {
    int quadID = gl_VertexID / 6;
    int vertexID = gl_VertexID % 6;
    vec4 geomData = texelFetch(spriteData, quadID);
    vec2 pos = verts[vertexID]*geomData.zw + geomData.xy + scale.xy;
    gl_Position = vec4(pos, 0.0, 1);//vec4(vert.x*scale.x+scale.z, vert.y*scale.y+scale.w, 0.0, 1.0);
    fragUV = uvs[vertexID];
}