///////////////////////////////////////////////////////////////////////////////
// Pixel shader for dithering
//
// Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#version 440

#define M_PI 3.1415926535897932384626433832795

const float pi = 3.14159265358979323846;

in vec2 vertTexCoord;

uniform uint height, width;

uniform sampler2D light_texture;

uniform sampler2D noise;
uniform uint px;
uniform uint py;
uniform uint firstPass;

out vec4 FragColor;

//const float kernel[25] = float[](0.0/12, 0.0/12, 1.0/12, 0.0/12, 0.0/12,
//                                 0.0/12, 1.0/12, 1.0/12, 1.0/12, 0.0/12,
//                                 1.0/12, 1.0/12, 0.0/12, 1.0/12, 1.0/12,
//                                 0.0/12, 1.0/12, 1.0/12, 1.0/12, 0.0/12,
//                                 0.0/12, 0.0/12, 1.0/12, 0.0/12, 0.0/12);

const float kernel[25] = float[]( 1.0/74,  4.0/74,  6.0/74,  4.0/74,  1.0/74,
                                  1.0/74,  4.0/74,  6.0/74,  4.0/74,  1.0/74,
                                  1.0/74,  4.0/74,  0.0/74,  4.0/74,  1.0/74,
                                  1.0/74,  4.0/74,  6.0/74,  4.0/74,  1.0/74,
                                  1.0/74,  4.0/74,  6.0/74,  4.0/74,  1.0/74);


float err(float i, float j, sampler2D errorSource = light_texture) {
  vec4 temp = texture(errorSource, vertTexCoord + vec2(i, j));
  return temp.y*(1.0-temp.z) - temp.y*temp.z;
}

float errNoise(float i, float j) {
  return err(i, j, noise);
}

void main()
{
    vec2 gBufferPosition = gl_FragCoord.xy/vec2(width,height);
    vec4 cur = texture(light_texture, gBufferPosition);

    float sum;

//    if (firstPass == 1)
//      sum = cur.x + (2*errNoise(-1.5,0) + 2*errNoise(1.5,0) + 2*errNoise(0,-1.5) + 2*errNoise(0,1.5) + errNoise(-1,-1) + errNoise(1,-1) + errNoise(-1,1) + errNoise(1,1))/12;
//    else
//      sum = cur.x + (2*err(-1.5,0) + 2*err(1.5,0) + 2*err(0,-1.5) + 2*err(0,1.5) + err(-1,-1) + err(1,-1) + err(-1,1) + err(1,1))/12;
//    

    if (firstPass == 1)
      sum = cur.x + kernel[ 0]*errNoise(-2,-2) + kernel[ 1]*errNoise(-1,-2) + kernel[ 2]*errNoise( 0,-2) + kernel[ 3]*errNoise( 1,-2) + kernel[ 4]*errNoise( 2,-2) +
                    kernel[ 5]*errNoise(-2,-1) + kernel[ 6]*errNoise(-1,-1) + kernel[ 7]*errNoise( 0,-1) + kernel[ 8]*errNoise( 1,-1) + kernel[ 9]*errNoise( 2,-1) +
                    kernel[10]*errNoise(-2, 0) + kernel[11]*errNoise(-1, 0) + kernel[12]*errNoise( 0, 0) + kernel[13]*errNoise( 1, 0) + kernel[14]*errNoise( 2, 0) +
                    kernel[15]*errNoise(-2, 1) + kernel[16]*errNoise(-1, 1) + kernel[17]*errNoise( 0, 1) + kernel[18]*errNoise( 1, 1) + kernel[19]*errNoise( 2, 1) +
                    kernel[20]*errNoise(-2, 2) + kernel[21]*errNoise(-1, 2) + kernel[22]*errNoise( 0, 2) + kernel[23]*errNoise( 1, 2) + kernel[24]*errNoise( 2, 2);
    else
      sum = cur.x + kernel[ 0]*err(-2,-2) + kernel[ 1]*err(-1,-2) + kernel[ 2]*err( 0,-2) + kernel[ 3]*err( 1,-2) + kernel[ 4]*err( 2,-2) +
                    kernel[ 5]*err(-2,-1) + kernel[ 6]*err(-1,-1) + kernel[ 7]*err( 0,-1) + kernel[ 8]*err( 1,-1) + kernel[ 9]*err( 2,-1) +
                    kernel[10]*err(-2, 0) + kernel[11]*err(-1, 0) + kernel[12]*err( 0, 0) + kernel[13]*err( 1, 0) + kernel[14]*err( 2, 0) +
                    kernel[15]*err(-2, 1) + kernel[16]*err(-1, 1) + kernel[17]*err( 0, 1) + kernel[18]*err( 1, 1) + kernel[19]*err( 2, 1) +
                    kernel[20]*err(-2, 2) + kernel[21]*err(-1, 2) + kernel[22]*err( 0, 2) + kernel[23]*err( 1, 2) + kernel[24]*err( 2, 2);

    int fx = int(gl_FragCoord.x);
    int fy = int(gl_FragCoord.y);

    vec4 new = ((sum > 0.5))?vec4(cur.x,1.0-sum,1.0,1.0):vec4(cur.x,sum-0.0,0.0,1.0);
    FragColor = ((fx % 3 == px)&&(fy % 3 == py))? new : cur;
}
