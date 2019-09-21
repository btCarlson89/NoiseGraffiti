#version 330

//  display
uniform vec2 res;

//  time
uniform float time;

//  scene
uniform int scene;

//  attributes
out vec4 fragColor;

//  noise and randomness
float rand(float n){return fract(sin(n) * 43758.5453123);}

float rand(vec2 n) { 
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

//  utils
float circle(in vec2 _st, in float _radius){
    vec2 dist = _st;// - vec2(1.0);
    return 1.-smoothstep(_radius - 0.02,
                         _radius,
                         sqrt(dot(dist, dist)));
}

//  main 
void main()
{
	vec2 uv = gl_FragCoord.xy / res.xy;

    if (scene == 0) {
        uv *= 2.0;
        uv -= 1.0;
        uv.x *= res.x / res.y;

        float a = 0.0;

        for(int i = 0; i < 12; ++i){
            vec2 rpos = uv - vec2(res.x / res.y, 1.0) * (2.0 * vec2(rand(i), rand(i + time)) - 1.0);
            float r = 0.125 + 0.5 * rand(i + 0.3 * time);
            r = pow(r, 1.2);
            a = max(a, circle(rpos, r));
        }

        a = clamp(a, 0.0, 1.0);

    	fragColor = vec4(a, a, a, 1.0);
    }
    else {
        fragColor = vec4(1.0);
    }
}