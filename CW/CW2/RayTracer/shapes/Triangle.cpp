/*
 * Triangle.cpp
 *
 *
 */
#include "Triangle.h"



namespace rt{

	Triangle::~Triangle() {};

	Hit Triangle::intersect(Ray ray) {

		Hit h;
		//-----------to be implemented -------------

		//std::cout << "Vertexs " << v0 << v1 << v2 << std::endl;
		//std::cout << "ray d " << ray.d << std::endl;

		Vec3f p0 = v0 - ray.o;
		Vec3f p1 = v1 - ray.o;
		Vec3f p2 = v2 - ray.o;

		//std::cout << "Shifted " << p0 << p1 << p2 << std::endl;

		int kz = ray.d.abs().MaxDimension();
		int kx = kz + 1; if (kx == 3) kx = 0;
		int ky = kx + 1; if (ky == 3) ky = 0;

		//std::cout << "Permutation " << kx << ky << kz << std::endl;


		Vec3f d = ray.d.Permute(kx, ky, kz);
		p0 = p0.Permute(kx, ky, kz);
		p1 = p1.Permute(kx, ky, kz);
		p2 = p2.Permute(kx, ky, kz);

		//std::cout << "Permuted " << d << p0 << p1 << p2 << std::endl;

		float Sx = -d.x / d.z;
		float Sy = -d.y / d.z;
		float Sz = 1.f / d.z;

		p0.x += Sx * p0.z;
		p0.y += Sy * p0.z;
		p1.x += Sx * p1.z;
		p1.y += Sy * p1.z;
		p2.x += Sx * p2.z;
		p2.y += Sy * p2.z;

		//std::cout << "Transformed " << p0 << p1 << p2 << std::endl;

		float e0 = p1.x * p2.y - p1.y * p2.x;
		float e1 = p2.x * p0.y - p2.y * p0.x;
		float e2 = p0.x * p1.y - p0.y * p1.x;

		//std::cout << "Edges " << e0 << e1 << e2 << std::endl;

		if ((e0 > 0 || e1 > 0 || e2 > 0) && (e0 < 0 || e1 < 0 || e2 < 0)) return h;
		float det = e0 + e1 + e2;
		if (det == 0) return h;

		p0.z *= Sz;
		p1.z *= Sz;
		p2.z *= Sz;

		float tScaled = e0 * p0.z + e1 * p1.z + e2 * p2.z;
		if (det < 0 && (tScaled >=0 || tScaled < ray.tMax * det)) return h;
		if (det > 0 && (tScaled <=0 || tScaled > ray.tMax * det)) return h;

		float b0 = e0 / det;
		float b1 = e1 / det;
		float b2 = e2 / det;
		h.tValue = tScaled / det;
		h.valid = true;
		h.object = this;
		h.point = ray(h.tValue);
		h.normal = (v0 - h.point).crossProduct(v1 - h.point).normalized();
		return h;

	}



} //namespace rt
