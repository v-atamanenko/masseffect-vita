uniform highp vec4 g_UniformColour;
uniform highp float g_LODBias;
uniform lowp sampler2D g_Texture0;
varying highp vec2 TexCoord0;
void main()
{
	gl_FragColor = (g_UniformColour * texture2D( g_Texture0, TexCoord0, g_LODBias ));
}
