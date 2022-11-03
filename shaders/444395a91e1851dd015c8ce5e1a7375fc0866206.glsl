uniform highp mat4 g_WorldViewProjection;
uniform highp vec4 g_UniformColour;
attribute highp vec3 vs_Position;
attribute lowp vec4 vs_Colour;
varying lowp vec4 Colour;
void main()
{
	Colour = (vs_Colour * g_UniformColour);
	gl_Position = (g_WorldViewProjection * vec4(vs_Position, 1.0));
}
