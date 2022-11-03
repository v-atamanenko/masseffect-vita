uniform highp mat4 g_WorldViewProjection;
uniform highp mat4 g_TextureTransform0;
uniform highp vec4 g_UniformColour;
attribute highp vec3 vs_Position;
attribute highp vec2 vs_TexCoord0;
attribute lowp vec4 vs_Colour;
varying highp vec2 TexCoord0;
varying lowp vec4 Colour;
void main()
{
	Colour = (vs_Colour * g_UniformColour);
	TexCoord0 = (g_TextureTransform0 * vec4(vs_TexCoord0, 0.0, 1.0)).xy;
	gl_Position = (g_WorldViewProjection * vec4(vs_Position, 1.0));
}
