uniform float time, x;
varying vec3 normal, lightDir;

void main(void)
{
    normal = normalize(gl_NormalMatrix * gl_Normal);
    lightDir = normalize(vec3(gl_LightSource[0].position));
    gl_TexCoord[0] = gl_MultiTexCoord0;

    vec4 v = vec4(gl_Vertex);

    if (v.x > x)
    {
        v.z += 0.3 * (v.x - x) * sin(3.0 * time);
    }

    gl_Position = gl_ModelViewProjectionMatrix * v;
}
