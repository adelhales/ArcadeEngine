#version 120

uniform sampler2D sourceTexture;
uniform vec2 texelSize;

void main()
{
    const vec2 offsets[4] = vec2[4](
        vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(1.0, -1.0)
    );

    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(sourceTexture, uv);

    for (int i = 0; i < 4; i++)
    {
        vec2 offset = texelSize * offsets[i];
        color += texture2D(sourceTexture, uv + offset);
        color += texture2D(sourceTexture, uv - offset);
    }

    gl_FragColor = color / 9.0;
}