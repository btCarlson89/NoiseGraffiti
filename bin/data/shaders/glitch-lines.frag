#version 330

uniform float time;
uniform float low;
uniform float divs;
uniform vec2 res;

uniform sampler2DRect tex;

out vec4 fragColor;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main()
{
	//	coords
	vec2 uv = gl_FragCoord.xy;
	vec2 p  = gl_FragCoord.xy / res.xy;

	//	glitch effect
	float fx = res.x * clamp(low, 0.0, 1.0);

	//	divs
	float rows = floor(divs);

	//	row
	float row = floor(p * rows).y;

	//	max offset
	float offset = random(vec2(row, time));

	//	threshold
	float mirror = random(vec2(time, row));

	if(mirror < 0.5)
		uv.x = clamp(uv.x, 0.0, res.x - fx * offset);
	else
		uv.x = clamp(uv.x, fx * offset, res.x);

    vec4 color = texture(tex, uv);


    fragColor = color;
}
