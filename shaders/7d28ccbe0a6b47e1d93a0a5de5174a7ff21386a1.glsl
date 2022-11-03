uniform highp mat4 g_WorldViewProjection;
uniform highp float g_TextureScale0;
uniform highp vec2 g_TextureBias0;
uniform highp vec3 g_CameraPosition;
uniform highp mat4 g_World;
uniform highp vec4 g_UniformColour;
uniform highp vec4 g_AmbientColor;
uniform lowp vec4 g_FresnelColor;
uniform highp float g_FresnelPower;
attribute highp vec3 vs_Position;
attribute highp vec2 vs_TexCoord0;
attribute highp vec3 vs_Normals;
varying highp vec2 TexCoord0;
varying highp vec3 CameraDir;
varying lowp vec4 UniformCol;
varying lowp vec4 FresnelCol;
void main()
{
	highp vec3 temporary;
	highp vec4 temporary2;
	temporary2 = vec4(vs_Position, 1.00);
	temporary = (g_CameraPosition - (g_World * temporary2).xyz);
	FresnelCol = vec4(g_FresnelColor.xyz * exp2(2.0 * g_FresnelPower * log2(1.01 - min(1.0, dot(normalize(temporary), normalize(mat3(g_World[0].xyz, g_World[1].xyz, g_World[2].xyz) * vs_Normals))))), 0);
	UniformCol = ((g_UniformColour) * (g_AmbientColor));
	CameraDir = temporary;
	TexCoord0 = ((vs_TexCoord0 * g_TextureScale0) + g_TextureBias0);
	gl_Position = (g_WorldViewProjection * temporary2);
}
