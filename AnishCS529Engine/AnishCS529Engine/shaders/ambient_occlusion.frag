///////////////////////////////////////////////////////////////////////////////
// Pixel shader for ambient occlusion
//
// Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#version 440

const float pi = 3.14159265358979323846;

layout (binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    mat4 inverseView;
    float exposure;
};

uniform uint height, width;

uniform sampler2D GBuffer_position;
uniform sampler2D GBuffer_normal;
uniform sampler2D GBuffer_eye_vector;

out float A;

void main()
{
    vec2 gBufferPosition = gl_FragCoord.xy/vec2(width,height);
    
    vec3 worldPos = texture(GBuffer_position, gBufferPosition).xyz;
    
    // AO code
    
    // Temporarily hard coding these for debugging
    float R = 1.0; // Range of influence
    int n = 10; // Number of samples
    float s = 1.0;  // scale of AO, taken from AAO
    float k = 10.0;  // contrast of AO, taken from AAO

    // Values for AO calculation
    float S_summ = 0.0;   // The values being summed. Not the same as S.
    float c = 0.1 * R;    // Falloff function reaches max at this distance.
    float delta = 0.001;  // Offset to avoid self occlusion

    // Values for Point generation
    int x_1 = int(gl_FragCoord.x);
    int y_1 = int(gl_FragCoord.y);
    float x = gBufferPosition.x;
    float y = gBufferPosition.y;

    vec3 P = worldPos;  // World Position
    vec3 N = texture(GBuffer_normal, gBufferPosition).xyz;  // Normal at point
    float d = texture(GBuffer_eye_vector, gBufferPosition).w;
        // Distance of point from eye

    // McGuire point selection
    // We use the below pseudo-random rotation calculated from int pixel
    // coefficients.
    // XOR always runs after mult.
    float phi = ( 30 * x_1 ^ y_1 ) + 10 * x_1 * y_1;

    // Calculating the points
    for (int i = 0; i < n; ++i) {
      float alpha = (i + 0.5)/n;  // Range 0 to 1, used for distance from
                                  // point.
      float h = alpha * R / d;    // Spiral radius. 
      // This is distance from point scaled from 0 to R/d
      // R/d is used here since it scales radius to world space.
      // (just R would be screen space).

      float theta = 2 * pi * alpha * ( 7 * n / 9 ) + phi;

      // Calculated point
      vec2 P_i = vec2(x,y) + h * vec2(cos(theta), sin(theta));

      // Calculating AO from the points
      {
        vec3 omega_i = texture(GBuffer_position, P_i).xyz - P;
        float d_i = texture(GBuffer_eye_vector, P_i).w;

        float influence = max(0.0, dot(N, omega_i) - delta * d_i);
        float H = step(0, R - length(omega_i));
        float scaling = max(pow(c,2), dot(omega_i, omega_i));

        S_summ += (influence * H / scaling);
      }
    }// End of Point calculation

    float S = ((2 * pi * c) / n) * S_summ;
      
    A = pow(max(0.0, (1 - s * S)), k);
}
