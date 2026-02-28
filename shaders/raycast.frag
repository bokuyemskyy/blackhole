#version 330 core

out vec4 FragColor;

uniform float uBlackHoleMass;

uniform mat4 uCameraInvViewProj;
uniform vec3 uCameraPosition;

uniform samplerCube uSkyboxTexture;
uniform float uSkyboxEscapeRadius;

uniform sampler2D uDiskTexture;
uniform float uDiskInnerRadius;
uniform float uDiskOuterRadius;
uniform float uDiskRotationSpeed;

uniform float uStepSize;
uniform int uMaxSteps;

uniform vec2 uResolution;
uniform float uTime;

const int max_steps = 1000;
const float min_step_size = 0.1;

float rs = 2 * uBlackHoleMass;
float three_m = 3 * uBlackHoleMass;
float r_isco = 3.0 * rs;

vec4 geodesic(float r, float v_r, float v_phi, float b) {
    return vec4(v_r, (b * b) * (1 - three_m / r) / (r * r * r), v_phi, -2 * v_r * v_phi / r);
}

vec4 rk4(float r, float v_r, float phi, float v_phi, float b, float h) {
    vec4 state1 = geodesic(r, v_r, v_phi, b);
    vec4 state2 = geodesic(r + (h / 2) * state1.x, v_r + (h / 2) * state1.y, v_phi + (h / 2) * state1.w, b);
    vec4 state3 = geodesic(r + (h / 2) * state2.x, v_r + (h / 2) * state2.y, v_phi + (h / 2) * state2.w, b);
    vec4 state4 = geodesic(r + h * state3.x, v_r + h * state3.y, v_phi + h * state3.w, b);

    float r_next = r + (h / 6.0) * (state1.x + 2.0 * state2.x + 2.0 * state3.x + state4.x);
    float v_r_next = v_r + (h / 6.0) * (state1.y + 2.0 * state2.y + 2.0 * state3.y + state4.y);
    float phi_next = phi + (h / 6.0) * (state1.z + 2.0 * state2.z + 2.0 * state3.z + state4.z);
    float v_phi_next = v_phi + (h / 6.0) * (state1.w + 2.0 * state2.w + 2.0 * state3.w + state4.w);
    return vec4(r_next, v_r_next, phi_next, v_phi_next);
}

vec3 getRayDirection() {
    vec2 ndc = (gl_FragCoord.xy / uResolution) * 2.0 - 1.0;
    vec4 clip = vec4(ndc, 1.0, 1.0);
    vec4 world = uCameraInvViewProj * clip;
    world /= world.w;
    return normalize(world.xyz - uCameraPosition);
}

vec4 sampleDisk(vec3 pos)
{
    float radius = length(pos.xz);

    if (radius < r_isco || radius > uDiskOuterRadius)
    return vec4(0.0);

    float rNorm = (radius - r_isco) / (uDiskOuterRadius - r_isco);
    rNorm = clamp(rNorm, 0.0, 1.0);

    float density = exp(-3.0 * rNorm);
    float innerFade = smoothstep(r_isco, r_isco * 1.2, radius);
    density *= innerFade;

    // Temperature color gradient
    vec3 hotColor = vec3(1.0, 0.9, 0.75);
    vec3 coolColor = vec3(1.0, 0.35, 0.1);
    vec3 baseColor = mix(hotColor, coolColor, rNorm);

    // Disk rotation
    float angle = uTime * uDiskRotationSpeed;
    mat2 rot = mat2(
    cos(angle), -sin(angle),
    sin(angle), cos(angle)
    );

    vec2 uv = pos.xz / (2.0 * uDiskOuterRadius);
    uv = rot * uv + 0.5;

    vec4 tex = textureLod(uDiskTexture, uv, 0.0);
    if (tex.a < 0.05)
    return vec4(0.0);

    // Doppler + redshift
    float v = sqrt(uBlackHoleMass / radius);
    vec3 diskVelocity = normalize(vec3(-pos.z, 0.0, pos.x));

    float gamma = 1.0 / sqrt(1.0 - v * v);
    float cosTheta = dot(diskVelocity, normalize(uCameraPosition - pos));
    float doppler = 1.0 / (gamma * (1.0 - v * cosTheta));

    float grav = sqrt(1.0 - rs / radius);

    // Total energy shift
    float energyShift = doppler * grav;

    // Simple emission model
    float emission = density * tex.a;
    emission *= pow(energyShift, 2.0);
    emission = 1.0 - exp(-emission * 3.0);

    // Final color
    vec3 color = baseColor * tex.rgb * emission;

    return vec4(color, emission);
}

void main() {
    vec3 p = uCameraPosition;
    vec3 rayDir = getRayDirection();

    float r0 = length(p);

    vec3 radial = p / r0;
    vec3 orbital = normalize(cross(radial, rayDir));
    vec3 tangent = normalize(cross(orbital, radial));

    float vr_local = dot(rayDir, radial);
    float sin_alpha = length(cross(radial, rayDir));
    float b = r0 * sin_alpha / sqrt(1.0 - rs / r0);

    float v_phi0 = b / (r0 * r0);
    float vr_sq = 1.0 - (b * b / (r0 * r0)) * (1.0 - rs / r0);
    float v_r0 = sign(vr_local) * sqrt(max(vr_sq, 0.0));

    float r = r0;
    float v_r = v_r0;
    float phi = 0.0;
    float v_phi = v_phi0;

    vec3 accumulatedColor = vec3(0.0);
    float transmission = 1.0;

    vec3 prevPos = p;
    for (int i = 0; i < max_steps; ++i) {
        float h = min_step_size * max(r * 0.1, 1);

        vec4 state = rk4(r, v_r, phi, v_phi, b, h);

        r = state.x;
        v_r = state.y;
        phi = state.z;
        v_phi = state.w;

        vec3 currentPos = r * (radial * cos(phi) + tangent * sin(phi));


        if (sign(prevPos.y) != sign(currentPos.y)) {
            float t = -prevPos.y / (currentPos.y - prevPos.y);
            vec3 diskPos = mix(prevPos, currentPos, t);
            float radius = length(diskPos);
            if (radius > r_isco && radius < uDiskOuterRadius) {
                vec4 tex = sampleDisk(diskPos);
                accumulatedColor += transmission * tex.rgb * tex.a;
                transmission *= (1.0 - tex.a);
            }
        }

        prevPos = currentPos;

        if (r < rs) {
            transmission = 0.0; break; }
        if (r > uSkyboxEscapeRadius) break;
        if (transmission < 0.01) break;
    }

    vec3 exitDir = r * (radial * cos(phi) + tangent * sin(phi));
    accumulatedColor += texture(uSkyboxTexture, normalize(exitDir)).rgb * transmission;

    FragColor = vec4(accumulatedColor, 1.0);
}