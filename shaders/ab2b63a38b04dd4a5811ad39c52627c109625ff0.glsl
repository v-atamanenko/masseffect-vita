uniform highp vec4 g_BonePalette[192];
uniform highp mat4 g_WorldViewProjection;
uniform highp vec4 g_PositionScaleBias;
uniform highp float g_TextureScale0;
uniform highp vec2 g_TextureBias0;
uniform highp vec3 g_CameraPosition;
uniform highp mat4 g_World;
uniform highp vec4 g_UniformColour;
uniform highp vec4 g_AmbientColor;
uniform lowp vec4 g_FresnelColor;
uniform highp float g_FresnelPower;
attribute highp vec3 vs_Position;
attribute highp vec4 vs_SkinIndex;
attribute highp vec4 vs_SkinWeight;
attribute highp vec2 vs_TexCoord0;
attribute highp vec3 vs_Normals;
varying highp vec2 TexCoord0;
varying highp vec3 CameraDir;
varying lowp vec4 UniformCol;
varying lowp vec4 FresnelCol;

mat4 GetSkinNMat4(vec4 indexes, vec4 weights, int n)
{
  vec4 row1 = vec4(0.0,0.0,0.0,0.0);
  vec4 row2 = vec4(0.0,0.0,0.0,0.0);
  vec4 row3 = vec4(0.0,0.0,0.0,0.0);
  for(int i=0;i<n;i++)
  {
    row1 += g_BonePalette[int(indexes[i])*3]*weights[i];
    row2 += g_BonePalette[int(indexes[i])*3+1]*weights[i];
    row3 += g_BonePalette[int(indexes[i])*3+2]*weights[i];
  }
  return mat4(row1.x,row2.x,row3.x,0.0,row1.y,row2.y,row3.y,0.0,row1.z,row2.z,row3.z,0.0,row1.w,row2.w,row3.w,1.0);
}

mat4 GetSkinMat4(int idx)
{
  idx*=3;
  return mat4(g_BonePalette[idx].x,g_BonePalette[idx+1].x,g_BonePalette[idx+2].x,0.0,g_BonePalette[idx].y,g_BonePalette[idx+1].y,g_BonePalette[idx+2].y,0.0,g_BonePalette[idx].z,g_BonePalette[idx+1].z,g_BonePalette[idx+2].z,0.0,g_BonePalette[idx].w,g_BonePalette[idx+1].w,g_BonePalette[idx+2].w,1.0);
}

void main()
{
	highp mat4 temporary;
	highp vec3 temporary2;
	highp vec4 temporary3;
	temporary = GetSkinNMat4(vs_SkinIndex,vs_SkinWeight,4);
	UniformCol = ((g_UniformColour) * (g_AmbientColor));
	TexCoord0 = ((vs_TexCoord0 * g_TextureScale0) + g_TextureBias0);
	temporary3 = temporary * vec4((vs_Position * g_PositionScaleBias.x + g_PositionScaleBias.yzw), 1.00);
	temporary2 = (g_CameraPosition - (g_World * temporary3).xyz);
	gl_Position = (g_WorldViewProjection * temporary3);
	FresnelCol = vec4(g_FresnelColor.xyz * exp2(2.0 * g_FresnelPower * log2(1.01 - min(1.0, dot(normalize(temporary2), mat3(g_World[0].xyz, g_World[1].xyz, g_World[2].xyz) * mat3(temporary[0].xyz, temporary[1].xyz, temporary[2].xyz) * vs_Normals)))), 0);
	CameraDir = temporary2;
}
