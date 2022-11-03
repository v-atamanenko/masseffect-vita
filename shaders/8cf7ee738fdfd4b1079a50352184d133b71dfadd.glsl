uniform highp float g_LODBias;
uniform lowp sampler2D g_Texture0;
varying lowp vec4 Colour;
varying highp vec2 TexCoord0;
void main()
{
	gl_FragColor = (Colour * texture2D( g_Texture0, TexCoord0, g_LODBias ));
}
