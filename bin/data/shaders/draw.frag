#version 330

uniform sampler2DRect inCol;

//	attributes
in vec2 texCoordVarying;

out vec4 fragColor;

float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
    return 1.-smoothstep(0,
                         _radius,
                         sqrt(dot(dist, dist)));
}

void main()
{
	vec4 col = texture(inCol, texCoordVarying.st).xyzw;
    //	Circle shading
	col.w *= circle(gl_PointCoord, 1.0);

    fragColor = col;
}