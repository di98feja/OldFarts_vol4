#include "noiseGenerator.h"

float NoiseGenerator::hash(float x, float y, float z) {
        int xi = static_cast<int>(std::floor(x));
        int yi = static_cast<int>(std::floor(y));
        int zi = static_cast<int>(std::floor(z));
        
        // Better hash with more mixing
        int n = xi + yi * 57 + zi * 113;
        n = (n << 13) ^ n;
        n = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
        
        return static_cast<float>(n) / static_cast<float>(0x7fffffff);
}

float NoiseGenerator::noise3D(float x, float y, float z) {
        // Get integer and fractional parts
        float ix = std::floor(x);
        float iy = std::floor(y);
        float iz = std::floor(z);
        
        float fx = x - ix;
        float fy = y - iy;
        float fz = z - iz;
        
        // Apply smooth interpolation curves
        float ux = smoothstep(fx);
        float uy = smoothstep(fy);
        float uz = smoothstep(fz);
        
        // Sample 8 corners of the cube
        float c000 = hash(ix,     iy,     iz);
        float c001 = hash(ix,     iy,     iz + 1);
        float c010 = hash(ix,     iy + 1, iz);
        float c011 = hash(ix,     iy + 1, iz + 1);
        float c100 = hash(ix + 1, iy,     iz);
        float c101 = hash(ix + 1, iy,     iz + 1);
        float c110 = hash(ix + 1, iy + 1, iz);
        float c111 = hash(ix + 1, iy + 1, iz + 1);
        
        // Interpolate along x
        float c00 = lerp(c000, c100, ux);
        float c01 = lerp(c001, c101, ux);
        float c10 = lerp(c010, c110, ux);
        float c11 = lerp(c011, c111, ux);
        
        // Interpolate along y
        float c0 = lerp(c00, c10, uy);
        float c1 = lerp(c01, c11, uy);
        
        // Interpolate along z
        return lerp(c0, c1, uz);
}

float NoiseGenerator::fbm(float x, float y, float z) {
        float result = 0.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f;
        
        for (int i = 0; i < SMOKE_LAYERS; ++i) {
            result += amplitude * noise3D(frequency * x, frequency * y, frequency * z);
            amplitude *= 0.5f;
            frequency *= 2.0f;
        }
        
        return result;
}
