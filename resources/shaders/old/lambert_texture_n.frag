#version 450
in vec4 worldPosition;
in vec3 worldNormal;
in vec2 uv;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 objectColor;
out vec4 fragColor;


uniform sampler2D textureUnitID;

void main () {
    vec3 lightDirection = normalize(lightPosition - worldPosition.xyz);
    float dotProduct = max(dot(lightDirection, normalize(worldNormal)), 0.0);
    vec4 diffuse = dotProduct * vec4(objectColor, 1.0);
    vec4 ambient = vec4(0.1);
    fragColor = ambient + diffuse + texture(textureUnitID, uv) * 0.5;
    //fragColor = vec4(uv,1.0,1.0); //test na funkcnost uv souřadnic
}