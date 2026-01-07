#version 450
#define MAX_LIGHTS 8
in vec4 worldPosition;
in vec3 worldNormal;
uniform vec3 viewPosition;
uniform vec3 objectColor;
out vec4 fragColor;


struct lightSource {
vec3 position;
vec3 color;
vec3 direction;
int type; // 0 = point, 1 = spotlight, 2 = reflector
float intensity;

};

uniform lightSource lights [MAX_LIGHTS];

uniform int numLights;

float attenuation ( float d , float c , float l , float q, float distance )
{

// d = distance from the light to the fragment
// c = constant attenuation factor
// l = linear attenuation factor
// q = quadratic attenuation factor
float att = 1.0 / ( c + l * d + q * d * d ) - distance;

return clamp( att , 0.0 , 1.0 );
}

void main () {

    vec4 finalColor = vec4(0.0);
    for(int i = 0; i < numLights; i++) {
        vec3 lightDirection = normalize(lights[i].position - worldPosition.xyz);
        if(dot(lightDirection, normalize(worldNormal)) <= 0.0)
            continue;
        vec3 reflectDir = reflect(-lightDirection, normalize(worldNormal));
        float specular = pow(max(dot(normalize(viewPosition), normalize(reflectDir)), 0.0), 32);
        float dotProduct = max(dot(lightDirection, normalize(worldNormal)), 0.0);
        vec4 diffuse = dotProduct * vec4( objectColor, 1.0);
        float att = attenuation( length(lights[i].position - worldPosition.xyz), 0.0, 0.9, 0.32, 0.2);

        float spot = dot(normalize(lights[i].direction), -lightDirection);

        if (spot < 0.85) {
            specular = 0; 
            diffuse = vec4(0.0);
        }
        spot=(spot-0.85)/(1-0.85);


        finalColor += (diffuse + (specular * vec4(lights[i].color, 1.0))) * lights[i].intensity * spot * (att);
    }

    vec4 ambient =  vec4( objectColor / 5.0f, 1.0);
    fragColor = ambient + finalColor;
}