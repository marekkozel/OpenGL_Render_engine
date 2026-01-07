#version 450
in vec3 worldPosition;
in vec3 worldNormal;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 objectColor;
out vec4 fragColor;
void main () {
    vec3 lightDirection = normalize(lightPosition - worldPosition);
    float dotProduct = max(dot(lightDirection, normalize(worldNormal)), 0.0);
    vec4 diffuse = dotProduct * vec4(objectColor, 1.0);
    vec4 ambient = vec4(objectColor / 5.0f, 1.0);
    fragColor = ambient + diffuse;
}