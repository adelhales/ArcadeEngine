#version 120

uniform sampler2D sourceTexture;
uniform vec2 texelSize;

void main()
{
    const float weights[5] = float[5](
        0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162
    );

    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(sourceTexture, uv) * weights[0];

    for (int i = 1; i < 5; i++)
    {
        vec2 offset = texelSize * float(i);
        color += texture2D(sourceTexture, uv + offset) * weights[i];
        color += texture2D(sourceTexture, uv - offset) * weights[i];
    }

    gl_FragColor = color;
}