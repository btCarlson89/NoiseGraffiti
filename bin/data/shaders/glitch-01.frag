#version 330

uniform float amp;
uniform float time;
uniform vec2 res;
uniform sampler2DRect tex;

in vec2 texCoordVarying;

out vec4 fragColor;

const float amount = 0.02;
const float speed = 0.01;

void main()
{
    if(amp == 0.0) {
        fragColor = texture(tex, gl_FragCoord.xy);
        return;
    }

    vec2 uv = gl_FragCoord.xy / res.xy;

    vec2 uvRed = uv;
    vec2 uvBlue = uv;

    float s = abs(0.6 + 0.4*sin(time * speed)) * amount;
    uvRed.x += s;
    uvBlue.x -= s;

    fragColor =  texture(tex, uv * res);
    fragColor.r = texture(tex, uvRed * res).r;
    fragColor.b = texture(tex, uvBlue * res).b;

    fragColor = mix(texture(tex, gl_FragCoord.xy), fragColor, amp);
}