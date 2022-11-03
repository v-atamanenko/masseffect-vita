uniform highp mat4 g_WorldViewProjection;
uniform highp float g_TextureScale0;
uniform highp vec2 g_TextureBias0;
uniform highp mat4 g_World;
uniform highp vec3 g_CameraPosition;
uniform highp vec4 g_UniformColour;
uniform highp vec4 g_AmbientColor;
attribute highp vec3 vs_Position;
attribute highp vec2 vs_TexCoord0;
attribute highp vec3 vs_Normals;
varying highp vec2 TexCoord0;
varying highp vec3 Normals;
varying highp vec3 CameraDir;
varying lowp vec4 UniformCol;
void main()
{
	highp vec4 temporary;
	temporary = vec4(vs_Position, 1.00);
	UniformCol = ((g_UniformColour) * (g_AmbientColor));
	CameraDir = (g_CameraPosition - (g_World * temporary).xyz);
	Normals = normalize(mat3(g_World[0].xyz, g_World[1].xyz, g_World[2].xyz) * vs_Normals);
	TexCoord0 = ((vs_TexCoord0 * g_TextureScale0) + g_TextureBias0);
	gl_Position = (g_WorldViewProjection * temporary);
}
