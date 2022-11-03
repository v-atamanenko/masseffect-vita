uniform highp float g_LODBias;
uniform lowp sampler2D g_Texture0;
uniform lowp sampler2D g_Texture1;
varying highp vec2 TexCoord0;
varying highp vec2 TexCoord1;
varying lowp vec4 UniformCol;
varying highp float FogFactor;
varying lowp vec4 FogFactorColor;
void main()
{
	gl_FragColor = ((((texture2D( g_Texture0, TexCoord0, g_LODBias ) * texture2D( g_Texture1, TexCoord1 )) * UniformCol) * FogFactor) + FogFactorColor);
}