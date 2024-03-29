#version 330

// ping pong inputs
uniform sampler2DRect inPos;
uniform sampler2DRect inVel;
uniform sampler2DRect inCol;

//  timing
uniform float elapsed;
uniform float time;

//  display
uniform vec2 res;

//  noise params
uniform float noiseScaleMin;
uniform float noiseScaleMax;
uniform float noiseFrequencyMin;
uniform float noiseFrequencyMax;
uniform float dZ;
uniform vec3 offset;

//  colors
uniform vec4 intColor;
uniform vec4 extColor;
uniform vec3 origin;
uniform float radius;
uniform sampler2DRect bg;

//  attributes
in vec2 texCoordVarying;

layout(location = 0) out vec4 posOut;
layout(location = 1) out vec4 velOut;
layout(location = 2) out vec4 colOut;

#define TWO_PI 6.28318530718

//  Noise
///////////////////////////////////////////////////////////////////////////
float rand(float n){return fract(sin(n) * 43758.5453123);}
float rand(vec2 n) {return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);}
vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}

float snoise(vec3 v){ 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //  x0 = x0 - 0. + 0.0 * C 
  vec3 x1 = x0 - i1 + 1.0 * C.xxx;
  vec3 x2 = x0 - i2 + 2.0 * C.xxx;
  vec3 x3 = x0 - 1. + 3.0 * C.xxx;

// Permutations
  i = mod(i, 289.0 ); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients
// ( N*N points uniformly over a square, mapped onto an octahedron.)
  float n_ = 1.0/7.0; // N=7
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}

///////////////////////////////////////////////////////////////////////////
void main()
{
    //  Read in data
    vec3 pos = texture(inPos, texCoordVarying.st).xyz;
    vec4 vel = texture(inVel, texCoordVarying.st);
    float fade = 1.0 - texture(bg, pos.xy).r;

    //  Apply noise
    float noiseScale = mix(noiseScaleMin, noiseScaleMax, fade);
    float noiseFrequency = mix(noiseFrequencyMin, noiseFrequencyMax, fade);

    vec3 nPos = vec3((pos.x + offset.x) / noiseScale, (pos.y + offset.y) / noiseScale, (pos.z + offset.z) / noiseScale);
    nPos += 0.01 * (time + snoise(pos));
    float theta = snoise(nPos) * TWO_PI * noiseFrequency;
    vec3 dir = vec3(cos(theta), sin(theta), dZ);

    //  Move
    dir.xy *= vel.xy;
    pos += dir;
    
    //  Wrap boundary
    if (pos.x > res.x + 10.0) {
        pos.x = -5.0;
        //pos.x = res.x * rand(gl_FragCoord.x + time);
        //pos.y = res.y * rand(gl_FragCoord.y + time);
    }
    else if (pos.x < -10.0) {
        pos.x = res.x + 5.0;
        //pos.x = res.x * rand(gl_FragCoord.x + time);
        //pos.y = res.y * rand(gl_FragCoord.y + time);
    }
    else if (pos.y > res.y + 10.0) {
        pos.y = -5.0;
        //pos.x = res.x * rand(gl_FragCoord.x + time);
        //pos.y = res.y * rand(gl_FragCoord.y + time);
    }
    else if (pos.y < -10.0) {
        pos.y = res.y + 5.0;
        //pos.x = res.x * rand(gl_FragCoord.x + time);
        //pos.y = res.y * rand(gl_FragCoord.y + time);
    }

    fade = 1.0 - texture(bg, pos.xy).r;
    
    posOut = vec4(pos, 1.0);
    velOut = texture(inVel, texCoordVarying.st);
    colOut = mix(intColor, extColor, fade);
    //colOut.xyz *= 0.5 + 0.5 * rand(gl_FragCoord.xy);
}