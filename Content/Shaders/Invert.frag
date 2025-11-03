#version 120

uniform sampler2D sourceTexture;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(sourceTexture, uv);

    gl_FragColor = vec4(1.0 - color.rgb, color.a);
}