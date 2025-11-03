#version 120

uniform sampler2D sourceTexture;
uniform vec2 resolution;
uniform float time;

const float BRIGHTNESS   = 0.03;
const float CONTRAST     = 0.50;
const float SATURATION   = 0.50;
const float VIGNETTE     = 0.15;
const float COLOR_ADJUST = 1.35;
const float SPEED        = 3.50;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    vec3 color = texture2D(sourceTexture, uv).rgb + BRIGHTNESS;
    color = clamp(color * CONTRAST + (color * color) * SATURATION, 0.0, 1.0);

    float vignette = 8.0 * uv.x * (1.0 - uv.x) * uv.y *  (1.0 - uv.y);
    color *= pow(vignette, VIGNETTE) * COLOR_ADJUST;

    float scanlines = 0.45 + 0.2 * sin(time * SPEED + uv.y * resolution.y * 1.5);
    color *= 0.8 + 0.3 * scanlines;

    gl_FragColor = vec4(color, 1.0);
}