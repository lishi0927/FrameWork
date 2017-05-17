#version 330 core

in vec2 uv;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;

uniform sampler2D btex;
uniform sampler2D blendtex0;
uniform sampler2D blendtex1;

out vec4 color;

const float maxAniso = 4;
const float maxAnisoLog2 = log2( maxAniso );
const float virtPagesWide = 32.0;
const float pageWidth = 256 + 8;
const float pageBorder = 4;
const float virtTexelsWide = virtPagesWide * ( pageWidth - 2 * pageBorder );


void main()
{
   if(uv.x <= 0.001 || uv.x >= 0.999 || uv.y <= 0.001 || uv.y >= 0.999) discard;

   vec2 tuv = uv * virtPagesWide;

   vec2 texcoords = uv * virtTexelsWide;

   vec2 dx = dFdx(texcoords);
   vec2 dy = dFdy(texcoords);

   float px = dot(dx, dx);
   float py = dot(dy, dy);

   float maxLod = 0.5 * log2(max(px, py));
   float minLod = 0.5 * log2(min(px, py));
   float anisoLOD = maxLod - min( maxLod - minLod, maxAnisoLog2);

   float trilinearFraction = fract(anisoLOD);

   vec4 scaleBias1, scaleBias2;
   scaleBias1 = textureLod( btex, uv, anisoLOD - 0.5);
   scaleBias2 = textureLod( btex, uv, anisoLOD + 0.5);

   vec2 physCoords1 = uv * scaleBias1.x + scaleBias1.zw;
   vec2 physCoords2 = uv * scaleBias2.x + scaleBias2.zw;
   
   vec4 blend0 = texture(blendtex0, physCoords1);
   vec4 blend1 = texture(blendtex1, physCoords1);
   vec4 blend0_1 = texture(blendtex0, physCoords2);
   vec4 blend1_1 = texture(blendtex1, physCoords2);

   blend0 = mix(blend0, blend0_1, trilinearFraction);
   blend1 = mix(blend1, blend1_1, trilinearFraction);

   vec4 color0 = texture(tex0, tuv);
   vec4 color1 = texture(tex1, tuv);
   vec4 color2 = texture(tex2, tuv);
   vec4 color3 = texture(tex3, tuv);
   vec4 color4 = texture(tex4, tuv);
   vec4 color5 = texture(tex5, tuv);

   color = vec4(0, 0, 0, 0);
   color += blend0.x * color0;
   color += blend0.y * color1;
   color += blend0.z * color2;
   color += blend1.x * color3;
   color += blend1.y * color4;
   color += blend1.z * color5;
   color.w = 1;
}