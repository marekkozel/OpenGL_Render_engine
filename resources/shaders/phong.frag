#version 450
#define MAX_LIGHTS 8
in vec3 worldPosition;
in vec3 worldNormal;
uniform vec3 viewPosition;
uniform vec3 objectColor;
out vec4 fragColor;

uniform vec3 lightPosition;

struct lightSource {
vec3 position;
vec3 color;
float intensity;

};

uniform lightSource lights [MAX_LIGHTS];

uniform int numLights;

float attenuation ( float d , float c , float l , float q )
{

// d = distance from the light to the fragment
// c = constant attenuation factor
// l = linear attenuation factor
// q = quadratic attenuation factor
float att = 1.0 / ( c + l * d + q * d * d );

return clamp( att , 0.0 , 1.0 );
}

void main () {

    vec4 finalColor = vec4(0.0);
    for(int i = 0; i < numLights; i++) {
        vec3 lightDirection = normalize(lights[i].position - worldPosition);
        if(dot(lightDirection, normalize(worldNormal)) <= 0.0)
            continue;
        vec3 reflectDir = reflect(-lightDirection, normalize(worldNormal));
        float specular = pow(max(dot(normalize(viewPosition), normalize(reflectDir)), 0.0), 32);
        float dotProduct = max(dot(lightDirection, normalize(worldNormal)), 0.0);
        vec4 diffuse = dotProduct * vec4( objectColor, 1.0);
        float attenuation = attenuation( length(lights[i].position - worldPosition), 1.0, 0.09, 0.032 );
        finalColor += (diffuse + (specular * vec4(lights[i].color, 1.0))) * attenuation * lights[i].intensity;
    }

    vec4 ambient =  vec4( objectColor / 5.0f, 1.0);
    fragColor = ambient + finalColor;
}