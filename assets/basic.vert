#version 150
uniform vec4 scale;

in vec2 vert;
in vec2 vertUV;

out vec2 fragUV;

void main() {
    gl_Position = vec4(vert.x*scale.x+scale.z, vert.y*scale.y+scale.w, 0.0, 1.0);
    fragUV = vertUV;
}