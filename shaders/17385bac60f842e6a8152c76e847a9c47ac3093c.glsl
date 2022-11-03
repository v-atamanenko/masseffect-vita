uniform highp float g_LODBias;
uniform lowp sampler2D g_Texture0;
uniform lowp sampler2D g_Texture1;
uniform lowp samplerCube g_CubeMap;
uniform highp float g_CubeMapLevel;
varying highp vec2 TexCoord0;
varying lowp vec4 UniformCol;
varying highp vec3 CameraDir;
varying highp vec3 Normals;
void main()
{
	gl_FragColor = ((texture2D( g_Texture0, TexCoord0, g_LODBias ) * UniformCol) + ((texture2D( g_Texture1, TexCoord0 ) * textureCube( g_CubeMap, reflect(normalize(CameraDir), normalize(Normals)) )) * g_CubeMapLevel));
}
