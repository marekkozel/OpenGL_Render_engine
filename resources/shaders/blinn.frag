#version 450
in vec3 worldPosition;
in vec3 worldNormal;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
out vec4 fragColor;
void main () {
    vec3 lightDirection = normalize(lightPosition - worldPosition);
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 halfwayDir = normalize(lightDirection + viewDirection);

    // Upgrade
    vec3 reflectDir = reflect(-lightDirection, normalize(worldNormal));
    float spec_phong = pow(max(dot(normalize(viewPosition), normalize(reflectDir)), 0.0), 64);
    
    float spec = pow(max(dot(normalize(worldNormal), normalize(halfwayDir)), 0.0), 8);
    vec3 specular = vec3(0.4) * spec;
    
    float dotProduct = max(dot(lightDirection, normalize(worldNormal)), 0.0);
    
    vec4 diffuse = dotProduct * vec4(objectColor, 1.0);
    
    vec4 ambient = vec4( objectColor / 5.0f, 1.0);
    
    fragColor = ambient + diffuse + vec4(specular, 1.0) ;
}