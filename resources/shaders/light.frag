#version 450
#define MAX_LIGHTS 8
in vec3 worldPosition;
in vec3 worldNormal;
uniform vec3 viewPosition;
uniform vec3 objectColor;
in vec2 uv;

uniform sampler2D textureUnitID;
uniform int textureEnabled;

out vec4 fragColor;

uniform vec3 lightPosition;

struct Material {
    vec3 ambient;   // Ka
    vec3 diffuse;   // Kd
    vec3 specular;  // Ks
    float shininess;// alpha
};
uniform Material material;

struct lightSource {
vec3 position;
vec3 color;
float intensity;
int type; // 0 = ambient, 1 = directional, 2 = point, 3 = reflector
vec3 direction;
float distance;

};

uniform lightSource lights [MAX_LIGHTS];

uniform int numLights;

float attenuation ( float d , float c , float l , float q, float distance)
{

// d = distance from the light to the fragment
// c = constant attenuation factor
// l = linear attenuation factor
// q = quadratic attenuation factor

if ( distance == 0.0 )
    return 0.0;

if(distance == 1.0f)
    return 1.0f;

distance = (1.0 - distance) * 0.5f;

float att = (1.0 / ( c + l * d + q * d * d ) ) - distance;

return clamp( att , 0.0 , 1.0);
}

void main () {

    vec3 editedObjectColor = objectColor * 0.8;

    vec3 matAmbient = material.ambient;
    vec3 matDiffuse = material.diffuse;
    vec3 matSpecular = material.specular;

    if(textureEnabled == 1) {
        vec4 texColor = texture(textureUnitID, uv);
        editedObjectColor = texColor.xyz;
      
    }

    vec4 finalColor = vec4(0.0);
    vec3 lightDirection = vec3(1.0);
    float att = 1.0;
    float spot = 1.0;
    vec4 ambient =  vec4( editedObjectColor * matAmbient, 1.0);
    float specular = 0.0;
    vec4 diffuse = vec4(0.0);
    for(int i = 0; i < numLights; i++) {

        if(lights[i].type == 0) {
            ambient =  vec4( editedObjectColor * lights[i].intensity * matAmbient, 1.0);
            continue;
        }
        else if(lights[i].type == 1) {
            lightDirection = normalize(vec3(1.0));
        }
        else if(lights[i].type == 2) {
            lightDirection = normalize(lights[i].position - worldPosition);
            att = attenuation( length(lights[i].position - worldPosition), 0.0, 0.9, 0.32, lights[i].distance );
        }
        else if(lights[i].type == 3) {
            lightDirection = normalize(lights[i].position - worldPosition);
            att = attenuation( length(lights[i].position - worldPosition), 0.0, 0.9, 0.32, lights[i].distance );

            spot = dot(normalize(lights[i].direction), -lightDirection);

        }

        if(lights[i].type == 1 || lights[i].type == 2 || lights[i].type == 3) {
            if(dot(lightDirection, normalize(worldNormal)) <= 0.0)
                continue;
            vec3 reflectDir = reflect(-lightDirection, normalize(worldNormal));
            specular = pow(max(dot(normalize(viewPosition), normalize(reflectDir)), 0.0), material.shininess);
            float dotProduct = max(dot(lightDirection, normalize(worldNormal)), 0.0);
            diffuse = dotProduct * vec4( editedObjectColor * matDiffuse, 1.0);

            if (lights[i].type == 3) {

                if (spot < 0.85) {
                specular = 0; 
                    diffuse = vec4(0.0);
                }
                spot=(spot-0.85)/(1-0.85);
            }
            
            finalColor += (diffuse + (specular * vec4(lights[i].color * matSpecular, 1.0))) * spot * att * lights[i].intensity;
        }
    
    }

    fragColor = ambient + finalColor;
}