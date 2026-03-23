#version 150

const vec2 verts[6] = vec2[6](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(0.0, 1.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0)
);

const vec2 uvs[6] = vec2[6](
    vec2(0.0, 1.0),
    vec2(1.0, 1.0),
    vec2(0.0, 0.0),
    vec2(1.0, 1.0),
    vec2(1.0, 0.0),
    vec2(0.0, 0.0)
);


uniform vec4 viewport;
uniform samplerBuffer spriteData;
uniform sampler2D tex;

out vec2 fragUV;
out float scale;
out float thickness;

void main() {
    int quadID = gl_VertexID / 6;
    int vertexID = gl_VertexID % 6;

    // input data
    vec4 dst = texelFetch(spriteData, quadID * 4);
    vec4 src = texelFetch(spriteData, quadID * 4 + 1);
    vec4 fd  = texelFetch(spriteData, quadID * 4 + 3);
    vec2 pos = verts[vertexID];

    // vars for conveniency
    float vx = viewport[0];
    float vy = viewport[1];
    float vw = viewport[2];
    float vh = viewport[3];

    float dx = dst[0];
    float dy = dst[1];
    float dw = dst[2];
    float dh = dst[3];

    float sx = src[0];
    float sy = src[1];
    float sw = src[2];
    float sh = src[3];

    vec2 texSize = textureSize(tex, 0);

    // position
    float px = (dx + dw * pos.x + vx) / (vw / 2.0 + vx) - 1.0;
    float py = 1.0 - (dy + dh * pos.y + vy) / (vh / 2.0 + vy);

    // texture coords
    float u = (sx + sw * pos.x) / texSize.x;
    float v = 1.0 - (sy + sh * pos.y) / texSize.y;

    gl_Position = vec4(px, py, 0.0, 1);
    fragUV = vec2(u, v);
    scale = fd[0];
    thickness = fd[1];
}