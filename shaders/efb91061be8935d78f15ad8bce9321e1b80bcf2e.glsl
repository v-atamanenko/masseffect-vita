uniform highp float g_LODBias;
uniform lowp sampler2D g_Texture0;
varying highp vec2 TexCoord0;
varying lowp vec4 UniformCol;
void main()
{
	gl_FragColor = (texture2D( g_Texture0, TexCoord0, g_LODBias ) * UniformCol);
}
