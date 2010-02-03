uniform sampler2D tex;
uniform bool textures, lighting;
varying vec3 normal, lightDir;

void main()
{
    vec3 ct,cf;
    vec4 texel;
    float intensity,at,af;

    if (lighting)
        intensity = max(dot(lightDir,normalize(normal)), 0.0);
    else
        intensity = 1.0;

    cf = intensity * (gl_FrontMaterial.diffuse).rgb +
                      gl_FrontMaterial.ambient.rgb;
    af = gl_FrontMaterial.diffuse.a;

    if (textures)
    {
        texel = texture2D(tex,gl_TexCoord[0].st);
        ct = texel.rgb;
        at = texel.a;
    }
    else
    {
        ct = vec3(1.0, 1.0, 1.0);
        at = 1.0;
    }

    gl_FragColor = vec4(ct * cf, at * af);
}
