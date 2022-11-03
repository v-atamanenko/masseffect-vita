uniform sampler2D    texture0;
uniform mediump int  textureUsed;
varying mediump vec2 varTexCoord0;
varying mediump vec4 varColor;

void main(void)        {
    if (textureUsed == 0)
    {
        gl_FragColor = varColor.rgba;
    }
    else
    {
        gl_FragColor = varColor.rgba*vec4(texture2D(texture0, varTexCoord0)).rgba;
    }
}