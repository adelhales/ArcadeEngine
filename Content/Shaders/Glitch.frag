#version 120

uniform sampler2D sourceTexture;
uniform float time;

const float FREQUENCY = 15.0;
const float SPEED     = 1.5;
const float AMPLITUDE = 0.005;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    float offsetX = sin(uv.y * FREQUENCY + time * SPEED) * AMPLITUDE;
    vec4 color = texture2D(sourceTexture, uv + vec2(offsetX, 0.0));

    gl_FragColor = color;
}