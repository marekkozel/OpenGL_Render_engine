#version 450
layout(location=0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vu;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 uv;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float w = 500.0;
uniform mat4 aMat4 = mat4(1.0); 

void main () {

        vec4 vp_project = vec4(vp * w , w);
        vec4 wp = aMat4 * modelMatrix * vp_project;
        worldPosition = wp.xyz / wp.w;
        
        mat3 normal = transpose(inverse(mat3(modelMatrix)));
        gl_Position = (projectionMatrix * viewMatrix *  aMat4 * modelMatrix) * vp_project;
        worldNormal = normal * vn;
        uv = vu;
}