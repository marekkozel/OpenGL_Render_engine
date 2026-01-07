#version 450
out vec4 fragColor;
in vec3 fragmentLocalPosition;
uniform samplerCube textureUnitID;

void main () {
     fragColor = texture(textureUnitID, fragmentLocalPosition) ;
}