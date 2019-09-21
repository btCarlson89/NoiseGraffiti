#version 330

uniform float amp;
uniform float time;
uniform vec2 res;
uniform sampler2DRect tex;

in vec2 texCoordVarying;

out vec4 fragColor;

#define PI 3.1415269
#define TWO_PI PI * 2.0
#define SIDES 1024.0

void main()
{
    if(amp == 0.0) {
        fragColor = texture(tex, gl_FragCoord.xy);
        return;
    }

    vec2 uv = gl_FragCoord.xy / res.xy;
    // Center UV
    vec2 p = uv - 0.5;
    p.x *= res.x / res.y;

    // Convert from cartesian coordinates to polar coordinates
    float r = 0.2 * length(p);        // r = âˆš( x2 + y2 )
    float angle = atan(p.y, p.x);   // Î¸ = tan-1 ( y / x )

    // Kaleidoscope effect
    angle = mod(angle, TWO_PI/(SIDES));
    angle = abs(angle - PI/(SIDES));

    // Convert from polar coordinates to cartesian coordinates
    p = r * vec2(cos(angle), sin(angle));   // x = r Ã— cos( Î¸ ), y = r Ã— sin( Î¸ )

    // Final color

    fragColor = texture(tex, res * (p - cos(time) / 10.0));

    fragColor = mix(texture(tex, gl_FragCoord.xy), fragColor, pow(amp, 0.5));
}