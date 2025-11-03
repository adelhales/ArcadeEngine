#version 120

uniform sampler2D sourceTexture;
uniform sampler2D bloomTexture;

const float BLOOM_STRENGTH = 0.70;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    vec4 sourceColor = texture2D(sourceTexture, uv);
    vec4 bloomColor  = texture2D(bloomTexture, uv);

    gl_FragColor = sourceColor + bloomColor * BLOOM_STRENGTH;
}