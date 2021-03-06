#version 330 core

in vec2 uv;

out vec4 color;

const float maxAniso = 4;
const float maxAnisoLog2 = log2( maxAniso );
const float virtPagesWide = 32.0;
const float pageWidth = 256 + 8;
const float pageBorder = 4;
const float virtTexelsWide = virtPagesWide * ( pageWidth - 2 * pageBorder );

void main()
{
   vec2 texcoords = uv * virtTexelsWide;

   vec2 dx = dFdx( texcoords );
   vec2 dy = dFdy( texcoords );

   float px = dot( dx, dx );
   float py = dot( dy, dy );

   float maxLod = 0.5 * log2( max( px, py ) );
   float minLod = 0.5 * log2( min( px, py ) );
   float anisoLOD = maxLod - min( maxLod - minLod, maxAnisoLog2 );

   color.xy = uv;
   color.z = anisoLOD;
   color.w = 1.0f;
}