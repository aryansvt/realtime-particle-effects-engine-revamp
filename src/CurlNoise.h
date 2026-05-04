#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include <cstdint>

// divergence free curl noise for fluid like particle motion
// based on bridson 2007 curl noise for procedural fluid flow
// the curl of any vector potential is divergence free
// so particles never compress or pile up along straight lines

namespace curl_noise {

    // smoothstep style fade for value noise
    inline float fade(float t) {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    // integer hash that returns a value in [-1 1]
    inline float hash3i(int x, int y, int z) {
        uint32_t n = (uint32_t)(x * 374761393) + (uint32_t)(y * 668265263) + (uint32_t)(z * 1274126177);
        n = (n ^ (n >> 13u)) * 1274126177u;
        n = n ^ (n >> 16u);
        return ((float)(n & 0x7FFFFFFFu) / (float)0x7FFFFFFF) * 2.0f - 1.0f;
    }

    // 3d trilinear value noise in [-1 1]
    inline float value3(float x, float y, float z) {
        const int xi = (int)std::floor(x);
        const int yi = (int)std::floor(y);
        const int zi = (int)std::floor(z);

        const float xf = x - (float)xi;
        const float yf = y - (float)yi;
        const float zf = z - (float)zi;

        const float u = fade(xf);
        const float v = fade(yf);
        const float w = fade(zf);

        const float c000 = hash3i(xi,     yi,     zi    );
        const float c100 = hash3i(xi + 1, yi,     zi    );
        const float c010 = hash3i(xi,     yi + 1, zi    );
        const float c110 = hash3i(xi + 1, yi + 1, zi    );
        const float c001 = hash3i(xi,     yi,     zi + 1);
        const float c101 = hash3i(xi + 1, yi,     zi + 1);
        const float c011 = hash3i(xi,     yi + 1, zi + 1);
        const float c111 = hash3i(xi + 1, yi + 1, zi + 1);

        const float x00 = c000 * (1.0f - u) + c100 * u;
        const float x10 = c010 * (1.0f - u) + c110 * u;
        const float x01 = c001 * (1.0f - u) + c101 * u;
        const float x11 = c011 * (1.0f - u) + c111 * u;

        const float y0 = x00 * (1.0f - v) + x10 * v;
        const float y1 = x01 * (1.0f - v) + x11 * v;

        return y0 * (1.0f - w) + y1 * w;
    }

    // three offset noise samples form a vector potential
    // each component uses a different domain offset so they decorrelate
    inline glm::vec3 potential(const glm::vec3& p) {
        return glm::vec3(
            value3(p.x,         p.y,         p.z        ),
            value3(p.x + 31.4f, p.y + 17.7f, p.z + 47.3f),
            value3(p.x + 63.2f, p.y + 89.4f, p.z + 23.1f)
        );
    }

    // curl computed via finite differences of the potential
    // returns a divergence free vector field
    inline glm::vec3 curl(const glm::vec3& p, float scale, float t, float eps = 0.1f) {
        // sample in scaled domain and add time as a 4th dimension via offset
        const glm::vec3 q = p * scale + glm::vec3(t * 0.5f, t * 0.4f, t * 0.6f);

        const glm::vec3 dx(eps, 0.0f, 0.0f);
        const glm::vec3 dy(0.0f, eps, 0.0f);
        const glm::vec3 dz(0.0f, 0.0f, eps);

        const glm::vec3 px = (potential(q + dx) - potential(q - dx)) * (1.0f / (2.0f * eps));
        const glm::vec3 py = (potential(q + dy) - potential(q - dy)) * (1.0f / (2.0f * eps));
        const glm::vec3 pz = (potential(q + dz) - potential(q - dz)) * (1.0f / (2.0f * eps));

        // curl  dPz dy - dPy dz   dPx dz - dPz dx   dPy dx - dPx dy
        return glm::vec3(
            py.z - pz.y,
            pz.x - px.z,
            px.y - py.x
        );
    }

}
