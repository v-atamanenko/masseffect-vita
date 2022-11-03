attribute vec2 inVertexPosition;
attribute vec4 inVertexColor;
attribute vec2 inTexCoord0;
uniform mediump mat4 uMvpMatrix;
varying mediump vec2 varTexCoord0;
varying mediump vec4 varColor;
void main(void)
{
    varColor     = inVertexColor/255.1;
    vec4 pos     = uMvpMatrix * vec4(inVertexPosition, 0.0, 1.0);
    varTexCoord0 = inTexCoord0;
    gl_Position  = pos;
}