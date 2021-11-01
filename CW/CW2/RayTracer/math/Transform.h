
#ifndef TRANSFORM_H_
#define TRANSFORM_H_

//#include "math/geometry.h"
#include "core/RayHitStructs.h"

namespace rt {

    class Transform {
    private:
       
    public:

        Matrix44f m, mInv;

        Transform() {};
        /*Transform(const Matrix44f mat) {
            m = Matrix44f(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
            mInv = m.inverse();
        };*/

        Transform(const Matrix44f& mat) : m(mat), mInv(m.inverse()) {};
        Transform(const Matrix44f& mat, const Matrix44f& mInv) : m(mat), mInv(mInv) {};

        Transform Inverse() { return Transform(mInv, m); };
        Transform Transpose() { return Transform(m.transposed(), mInv.transposed()); };

        inline Vec3f TransformPoint(const Vec3f p) {
            Vec3f ret{};
            m.multVecMatrix(p, ret);
            return ret;
        }

        inline Vec3f TransformDirection(const Vec3f d) {
            Vec3f ret{};
            m.multDirMatrix(d, ret);
            return ret;
        }

        inline Vec3f TransformNormal(const Vec3f n) {
            Vec3f ret{};
            mInv.multDirMatrix(n, ret);
            return ret;
        }

        inline Ray TransformRay(const Ray& r) {
            Vec3f o = TransformPoint(r.o);
            Vec3f d = TransformDirection(r.d);
            return Ray(o, d);
        };

        Transform operator* (const Transform& t2) const {
            return Transform(m * t2.m, mInv * t2.mInv);
        }

        static Transform Translate(const Vec3f& d) {
            Matrix44f mt{};
            mt[0][3] = d.x;
            mt[1][3] = d.y;
            mt[2][3] = d.z;
            Matrix44f minvt{};
            minvt[0][3] = -d.x;
            minvt[1][3] = -d.y;
            minvt[2][3] = -d.z;
            return Transform(mt, minvt);
        };

        static Transform Scale(float x, float y, float z) {
            Matrix44f mt{};
            mt[0][0] = x;
            mt[1][1] = y;
            mt[2][2] = z;
            Matrix44f minvt{};
            minvt[0][0] = 1 / x;
            minvt[1][1] = 1 / y;
            minvt[2][2] = 1 / z;
            return Transform(mt, minvt);
        };

        static Transform LookAt(const Vec3f& pos, const Vec3f& look, const Vec3f& up) {

            std::cout << pos << look << up << std::endl;

            Matrix44f mt{};
            mt[0][3] = pos.x;
            mt[1][3] = pos.y;
            mt[2][3] = pos.z;
            mt[3][3] = 1;
            Vec3f dir = (look - pos).normalized();
            Vec3f left = up.normalized().crossProduct(dir).normalized();
            Vec3f newUp = dir.crossProduct(left);
            std::cout << dir << left << newUp << std::endl;

            mt[0][0] = left.x;
            mt[1][0] = left.y;
            mt[2][0] = left.z;
            mt[3][0] = 0;
            mt[0][1] = newUp.x;
            mt[1][1] = newUp.y;
            mt[2][1] = newUp.z;
            mt[3][1] = 0;
            mt[0][2] = dir.x;
            mt[1][2] = dir.y;
            mt[2][2] = dir.z;
            mt[3][2] = 0;

            std::cout << mt << std::endl;

            return Transform(mt, mt.inverse());
        }

        static Transform Perspective(float fov, float n, float f) {
           Matrix44f persp(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, f / (f - n), -f * n / (f - n),
                0, 0, 1, 0);
           float invTanAng = 1.f / tan(2 * M_PI * (fov / 720));
            return Scale(invTanAng, invTanAng, 1) * Transform(persp);

        }

        template<typename T>
        static T Clamp(T a, T b, T c) {
            if (a > c) return c;
            if (a < b) return b;
            return a;
        }

        template<typename T>
        static Vec3<T> Clamp(Vec3<T> a, T b, T c) {
            if (a.x > c) a.x = c;
            if (a.x < b) a.x = b;
            if (a.y > c) a.y = c;
            if (a.y < b) a.y = b;
            if (a.z > c) a.z = c;
            if (a.z < b) a.z = b;
            return a;
        }

    };

}
#endif