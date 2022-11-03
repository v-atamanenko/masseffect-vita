uniform highp mat4 g_WorldViewProjection;
uniform highp float g_TextureScale0;
uniform highp vec2 g_TextureBias0;
uniform highp vec4 g_UniformColour;
uniform highp vec4 g_AmbientColor;
attribute highp vec3 vs_Position;
attribute highp vec2 vs_TexCoord0;
varying highp vec2 TexCoord0;
varying lowp vec4 UniformCol;
void main()
{
	UniformCol = ((g_UniformColour) * (g_AmbientColor));
	TexCoord0 = ((vs_TexCoord0 * g_TextureScale0) + g_TextureBias0);
	gl_Position = (g_WorldViewProjection * vec4(vs_Position, 1.00));
}
