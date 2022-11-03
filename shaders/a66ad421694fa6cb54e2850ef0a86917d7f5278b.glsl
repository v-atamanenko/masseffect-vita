uniform highp float g_LODBias;
uniform lowp sampler2D g_Texture0;
varying highp vec2 TexCoord0;
varying lowp vec4 UniformCol;
varying lowp vec4 FresnelCol;
varying highp float FogFactor;
varying lowp vec4 FogFactorColor;
void main()
{
	gl_FragColor = ((((texture2D( g_Texture0, TexCoord0, g_LODBias ) * UniformCol) + FresnelCol * fract(gl_FragCoord.y * 0.2)) * FogFactor) + FogFactorColor);
}
