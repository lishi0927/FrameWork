#version 330 core

layout (location = 0) in vec3 position;

uniform vec4 scale;
uniform vec4 offset;
uniform vec4 currentPos;
uniform mat4 gWVP;

uniform sampler2D vtex;
uniform sampler2D texHeightmap;

out vec2 uv;


const float VIEWCHUNKNUMBER = 20.0;
const float MAXSCALE = 100.0 * VIEWCHUNKNUMBER / 4.0;
const float CHUNKNUMBER = 32.0;
const float CHUNKSIZE = 51;

void main()
{
  vec4 pos = vec4(scale.xyz * (offset.xyz + position), 1.0);
  uv = pos.xz / MAXSCALE / 4.0 * VIEWCHUNKNUMBER /CHUNKNUMBER + currentPos.xy;
  vec4 scaleBias = texture(vtex, uv);
  vec2 pCoor = uv * scaleBias.x + scaleBias.zw;
  pos.y = texture(texHeightmap, pCoor).r;

  gl_Position = gWVP * pos;
}