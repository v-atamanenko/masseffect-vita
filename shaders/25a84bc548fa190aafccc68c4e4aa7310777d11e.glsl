uniform highp mat4 g_WorldViewProjection;
uniform highp mat4 g_TextureTransform0;
attribute highp vec3 vs_Position;
attribute highp vec2 vs_TexCoord0;
varying highp vec2 TexCoord0;
void main()
{
	TexCoord0 = (g_TextureTransform0 * vec4(vs_TexCoord0, 0.0, 1.0)).xy;
	gl_Position = (g_WorldViewProjection * vec4(vs_Position, 1.0));
}
