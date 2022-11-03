uniform highp float g_LODBias;
uniform lowp sampler2D g_Texture0;
varying highp vec2 TexCoord0;
varying lowp vec4 UniformCol;
varying highp float FogFactor;
varying lowp vec4 FogFactorColor;
void main()
{
	lowp vec4 temporary;
	temporary = (texture2D( g_Texture0, TexCoord0, g_LODBias ) * UniformCol);
	gl_FragColor = vec4(((temporary.xyz * FogFactor) + FogFactorColor.xyz), temporary.a);
}
