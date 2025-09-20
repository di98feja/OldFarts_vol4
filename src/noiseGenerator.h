#pragma once
#include <cmath>
#include <vector>
#include <random>

class NoiseGenerator {
private:
    static constexpr int SMOKE_LAYERS = 5;  // Adjust as needed
    
    float hash(float x, float y, float z);
    inline float fastHash(int x, int y, int z) {
        int n = x + y * 57 + z * 113;
        n ^= n << 13;
        n *= (n * n * 15731 + 789221);
        return ((n & 0x7fffffff) * 4.6566128752457969e-10f); // * (1/2^31)
    }

    inline float smoothstep(float t) {
        return t * t * (3.0f - 2.0f * t);
    }
    inline float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }
    // 3D noise function
    float noise3D(float x, float y, float z);

  public:
    // Fractional Brownian Motion
    float fbm(float x, float y, float z);

    // 2D version if you need it
    float fbm2D(float x, float y) {
        return fbm(x, y, 0.0f);
    }
};