#version 450
layout(location=0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vu;
out vec4 worldPosition;
out vec3 worldNormal;
out vec2 uv;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main () {
        mat3 normal = transpose(inverse(mat3(modelMatrix)));
        gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4 (vp, 1.0);
        worldPosition = modelMatrix * vec4(vp,1.0f);
        worldNormal = normal * vn;
        uv = vu;
}