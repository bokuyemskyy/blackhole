#version 330 core
out vec4 FragColor;
in vec2 uv;

uniform vec2 uResolution;
uniform samplerCube uSkybox;
uniform mat4 uCameraInverse;

void main()
{
    vec2 ndc = uv * 2.0 - 1.0;
    ndc.x *= uResolution.x / uResolution.y;

    vec4 rayCam = vec4(ndc.x, ndc.y, -1.0, 0.0); 
    vec3 rayDir = normalize((uCameraInverse * rayCam).xyz);

    vec3 color = texture(uSkybox, rayDir).rgb;

    FragColor = vec4(color, 1.0);
}