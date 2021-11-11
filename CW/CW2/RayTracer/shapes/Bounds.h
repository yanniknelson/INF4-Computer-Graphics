#ifndef BOUNDS_H_
#define BOUNDS_H_

#include "math/geometry.h"
#include "core/RayHitStructs.h"

template <typename T>
class Bounds2 {
public:
    Vec2<T> pMin;
    Vec2<T> pMax;

    Bounds2() {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();
        pMin = { maxNum, maxNum };
        pMax = { minNum, minNum };
    }

    Bounds2(const Vec2<T>& p) : pMin(p), pMax(p) {};

    Bounds2(const Vec2<T>& p1, const Vec2<T>& p2) : pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y)), pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y)) {};
};

template <typename T>
class Bounds3 {
public:
    Vec3<T> pMin;
    Vec3<T> pMax;

    Bounds3() {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();
        pMin = { maxNum, maxNum, maxNum };
        pMax = { minNum, minNum, minNum };
        
    }

    Bounds3(const Vec3<T>& p) : pMin(p), pMax(p) {};
    Bounds3(const Vec3<T>& p1, const Vec3<T>& p2) : pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)), pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)) {};

    const Vec3<T>& operator[](int i) const {
        return (i == 0) ? pMin : pMax;
    }
    Vec3<T>& operator[](int i) {
        return (i == 0) ? pMin : pMax;
    }

    Vec3<T> Corner(int corner) const {
        return Vec3<T>((*this)[(corner & 1)].x, (*this)[(corner & 2) ? 1 : 0].y, (*this)[(corner & 4) ? 1 : 0].z);
    }

    Bounds3<T> Union(const Bounds3<T>& b, const Vec3<T>& p) {
        return Bounds3<T>(Vec3<T>(std::min(b.pMin.x, p.x), std::min(b.pMin.y, p.y), std::min(b.pMin.z, p.z)),
            Vec3<T>(std::max(b.pMax.x, p.x), std::max(b.pMax.y, p.y), std::max(b.pMax.z, p.z)));
    }

    Bounds3<T> Union(const Bounds3<T>& b1, const Bounds3<T>& b2) {
        return Bounds3<T>(Vec3<T>(std::min(b1.pMin.x, b2.pMin.x), std::min(b1.pMin.y, b2.pMin.y), std::min(b1.pMin.z, b2.pMin.z)),
            Vec3<T>(std::max(b1.pMax.x, b2.pMax.x), std::max(b1.pMax.y, b2.pMax.y), std::max(b1.pMax.z, b2.pMax.z)));
    }

    Bounds3<T> Intersect(const Bounds3<T>& b1, const Bounds3<T>& b2) {
        return Bounds3<T>(Vec3<T>(std::max(b1.pMin.x, b2.pMin.x), std::max(b1.pMin.y, b2.pMin.y), std::max(b1.pMin.z, b2.pMin.z)),
            Vec3<T>(std::min(b1.pMax.x, b2.pMax.x), std::min(b1.pMax.y, b2.pMax.y), std::min(b1.pMax.z, b2.pMax.z)));
    }

    inline bool IntersectP(const rt::Ray& ray, float* hit0, float* hit1) const {
        float t0 = 0;
        float t1 = ray.tMax;
        for (int i = 0; i < 3; i++) {
            float invRayDir = 1 / ray.d[i];
            float tNear = (pMin[i] - ray.o[i]) * invRayDir;
            float tFar = (pMax[i] - ray.o[i]) * invRayDir;
            if (tNear > tFar) std::swap(tNear, tFar);
            tFar *= 1 + 2 * ((3* std::numeric_limits<T>::epsilon())/(1-3* std::numeric_limits<T>::epsilon()));
            t0 = tNear > t0 ? tNear : t0;
            t1 = tFar < t1 ? tFar : t1;
            if (t0 > t1) return false;
        }
        if (hit0) *hit0 = t0;
        if (hit1) *hit1 = t1;
        return true;
    }

    inline bool IntersectP(const rt::Ray& ray, const Vec3f& invDir, const int dirIsNeg[3]) const {
        const Bounds3& bounds = *this;
        float tMin = (bounds[dirIsNeg[0]].x - ray.o.x) * invDir.x;
        float tMax = (bounds[1-dirIsNeg[0]].x - ray.o.x) * invDir.x;
        float tyMin = (bounds[dirIsNeg[1]].y - ray.o.y) * invDir.y;
        float tyMax = (bounds[1-dirIsNeg[1]].y - ray.o.y) * invDir.y;
        if (tMin > tyMax || tyMin > tMax) return false;
        if (tyMin > tMin) tMin = tyMin;
        if (tyMax < tMax) tMax = tyMax;

        float tzMin = (bounds[dirIsNeg[2]].z - ray.o.z) * invDir.z;
        float tzMax = (bounds[1-dirIsNeg[2]].z - ray.o.z) * invDir.z;
        if (tMin > tzMax || tzMin > tMax) return false;
        if (tzMin > tMin) tMin = tzMin;
        if (tzMax < tMax) tMax = tzMax;

        return (tMin < ray.tMax) && (tMax > 0);
    }

    bool Overlaps(const Bounds3<T>& b1, const Bounds3<T>& b2) {
        bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
        bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
        bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
        return (x && y && z);
    }

    bool Inside(const Vec3<T>& p, const Bounds3<T>& b) {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x &&
            p.y >= b.pMin.y && p.y <= b.pMax.y &&
            p.z >= b.pMin.z && p.z <= b.pMax.z);
    }

    bool InsideExclusive(const Vec3<T>& p, const Bounds3<T>& b) {
        return (p.x >= b.pMin.x && p.x < b.pMax.x&&
            p.y >= b.pMin.y && p.y < b.pMax.y&&
            p.z >= b.pMin.z && p.z < b.pMax.z);
    }

    Bounds3<T> Expand(const Bounds3<T>& b, T delta) {
        return Bounds3<T>(b.pMin - Vec3<T>(delta), b.pMax + Vec3<T>(delta));
    }

    Vec3<T> Diagonal() const { 
        if (pMax[pMax.MinDimension()] == std::numeric_limits<T>::lowest() || pMin[pMin.MaxDimension()] == std::numeric_limits<T>::max()) {
            return Vec3<T>();
        }
        return pMax - pMin; 
    };

    T SurfaceArea() const {
        Vec3<T> d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    T Volume() const {
        Vec3<T> d = Diagonal();
        return d.x * d.y * d.z;
    }

    int MaximumExtent() {
        Vec3<T> d = Diagonal();
        return d.MaxDimension();
    }

    inline float Lerp(float t, float v1, float v2) {
        return (1 - t) * v1 + t * v2;
    }

    Vec3<T> Lerp(const Vec3<T>& t) {
        return Vec3<T>(Lerp(t.x, pMin.x, pMax.x), Lerp(t.y, pMin.y, pMax.y), Lerp(t.z, pMin.z, pMax.z));
    }

    Vec3<T> Offset(const Vec3<T>& p) {
        Vec3<T> o = p - pMin;
        if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
        if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
        return o;
    }

};

typedef Bounds2<float> Bounds2f;
typedef Bounds3<float> Bounds3f;

#endif /* GEOMETRY_H_ */