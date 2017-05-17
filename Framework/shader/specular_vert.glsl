#version 330 core

layout (location = 0) in vec3 position;

const int NUM_CASCADES = 3;

uniform vec4 scale;
uniform vec4 offset;
uniform vec4 currentPos;
uniform mat4 gWVP;
uniform mat4 gLightWVP[NUM_CASCADES];

uniform sampler2D vtex;
uniform sampler2D texHeightmap;
//uniform sampler2D texNormalmap;
uniform sampler2D AOmap;

uniform mat4 gWorld;

out vec2 uv;
out vec3 WorldPos0;
out vec3 Normal0;
out float AO;
out vec4 LightSpacePos[NUM_CASCADES];
out float ClipSpacePosZ;
out float scale0;

const float VIEWCHUNKNUMBER = 20.0;
const float MAXSCALE = 100.0 * VIEWCHUNKNUMBER / 4.0;
const float CHUNKNUMBER = 32.0;
const float CHUNKSIZE = 51;
const vec2 size = vec2(2.0,0.0);
const ivec3 off = ivec3(-1,0,1);

void main()
{
  vec4 pos = vec4(scale.xyz * position + offset.xyz, 1.0);
  uv = pos.xz / MAXSCALE / 4.0 * VIEWCHUNKNUMBER /CHUNKNUMBER + currentPos.xy;
  vec4 scaleBias = texture(vtex, uv);
  vec2 pCoor = uv * scaleBias.x + scaleBias.zw;
  pos.y = texture(texHeightmap, pCoor).r;

  float s01 = textureOffset(texHeightmap, pCoor, off.xy).r;
  float s21 = textureOffset(texHeightmap, pCoor, off.zy).r;
  float s10 = textureOffset(texHeightmap, pCoor, off.yx).r;
  float s12 = textureOffset(texHeightmap, pCoor, off.yz).r;
  vec3 va = normalize(vec3(size.x,s21-s01,size.y));
  vec3 vb = normalize(vec3(size.y,s12-s10,size.x));
  Normal0 = normalize(cross(va,vb));
  Normal0 = (gWorld * vec4(Normal0, 0.0)).xyz;

  gl_Position = gWVP * pos;
  WorldPos0 = (gWorld * pos).xyz;

 //Normal0 = texture(texNormalmap, pCoor).rgb;
 //Normal0 = vec3(2.0 *Normal0.x-1.0, 2.0 *Normal0.y-1.0,2.0 *Normal0.z-1.0);
 //Normal0 = normalize(Normal0);
 //Normal0 = (gWorld * vec4(Normal0, 0.0)).xyz;
 
  AO = clamp(texture(AOmap, pCoor).r,0.0,1.0);

   for (int i = 0 ; i < NUM_CASCADES ; i++) {
        LightSpacePos[i] = gLightWVP[i] * pos;
    }

	ClipSpacePosZ = gl_Position.z;

	scale0 = scale.x / MAXSCALE;
}