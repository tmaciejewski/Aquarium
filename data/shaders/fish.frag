#version 130

uniform sampler2D tex;
uniform bool textures, lighting;
varying vec3 normal;

void main()
{
    vec4 cf, ct;
    vec4 diffuse, ambient;
    float intensity;

    if (lighting)
    {
        vec4 position = gl_ModelViewMatrix * vec4(0.0);
        intensity = dot(vec3(gl_LightSource[0].position), normalize(normal));

        if (intensity > 1.0)
            intensity = 1.0;

        diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
        ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    }
    else
    {
        diffuse = gl_FrontMaterial.diffuse;
        ambient = gl_FrontMaterial.ambient;
        intensity = 1.0;
    }

    cf = intensity * diffuse + ambient;

    if (textures && textureSize(tex, 0) != ivec2(0, 0))
    {
        ct = texture2D(tex, gl_TexCoord[0].st);
    }
    else
    {
        ct = vec4(1.0);
    }

    gl_FragColor = ct * cf;
}
