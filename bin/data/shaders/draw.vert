#version 330

uniform mat4 modelViewProjectionMatrix;
uniform sampler2DRect inPos;

in vec4  position;
in vec2  texcoord;

out vec2 texCoordVarying;

void main()
{
    texCoordVarying = texcoord;
    vec3 pos = texture(inPos, texCoordVarying).xyz;
    pos.z = 0.0;
    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
}