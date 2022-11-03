uniform highp mat4 g_WorldViewProjection;
uniform highp float g_TextureScale0;
uniform highp vec2 g_TextureBias0;
uniform highp float g_TextureScale1;
uniform highp vec2 g_TextureBias1;
uniform highp vec4 g_UniformColour;
uniform highp vec4 g_AmbientColor;
uniform highp float g_LightMapLevel;
uniform highp float g_FogEnd;
uniform highp float g_FogRangeInv;
uniform lowp vec4 g_FogColor;
attribute highp vec3 vs_Position;
attribute highp vec2 vs_TexCoord0;
attribute highp vec2 vs_TexCoord1;
attribute lowp vec4 vs_Colour;
varying highp vec2 TexCoord0;
varying highp vec2 TexCoord1;
varying lowp vec4 UniformCol;
varying highp float FogFactor;
varying lowp vec4 FogFactorColor;
void main()
{
	highp float temporary;
	highp vec4 temporary2;
	temporary2 = (g_WorldViewProjection * vec4(vs_Position, 1.00));
	temporary = clamp((g_FogEnd - temporary2.w) * g_FogRangeInv, 0.0, 1.0);
	FogFactorColor = ((1.0 - temporary) * g_FogColor);
	FogFactor = temporary;
	UniformCol = ((((g_UniformColour) * (g_AmbientColor)) * g_LightMapLevel) * vs_Colour);
	TexCoord1 = ((vs_TexCoord1 * g_TextureScale1) + g_TextureBias1);
	TexCoord0 = ((vs_TexCoord0 * g_TextureScale0) + g_TextureBias0);
	gl_Position = temporary2;
}
