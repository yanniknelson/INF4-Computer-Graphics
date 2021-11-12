/*
 * TriMesh.cpp
 *
 *
 */
#include "TriMesh.h"
#include "parsers/hpply.h"



namespace rt{

	TriMesh::TriMesh(std::vector<std::shared_ptr<Shape>> inputfaces, Vec3f position, float scale, Material* mat) {
		scale = scale;
		name = "Trimesh";

		std::cout << "Building TriMesh" << std::endl;

		ObjectToWorld = Transform::Translate(position) * Transform::Scale(scale, scale, scale);
		WorldToObject = ObjectToWorld.Inverse();

		position = position;
		material = mat;

		faces = inputfaces;

	for (int i = 0; i < faces.size(); i++){
		ObjectBound = ObjectBound.Union(ObjectBound, faces[i]->getWorldBounds());
	}

	VolumeRoot = BVH::BuildTree(faces);
	WorldBound = ObjectToWorld.TransformBounds3(ObjectBound);
	}


	TriMesh::TriMesh(std::string path, Vec3f position, float scale, Material* mat) {
		scale = scale;
		name = "Trimesh";
	
		std::cout << "Building TriMesh" << std::endl;

		ObjectToWorld = Transform::Translate(position) * Transform::Scale(scale, scale, scale);
		WorldToObject = ObjectToWorld.Inverse();

		position = position;
		material = mat;

		happly::PLYData plyIn(path);


		std::vector<double> nxs;
		std::vector<double> nys;
		std::vector<double> nzs;

		std::vector<double> ts;
		std::vector<double> ss;

		std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
		std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();

		bool hasNormals = false;
		if (plyIn.getElement("vertex").hasProperty("nx")) {
			hasNormals = true;
			nxs = plyIn.getElement("vertex").getProperty<double>("nx");
			nys = plyIn.getElement("vertex").getProperty<double>("ny");
			nzs = plyIn.getElement("vertex").getProperty<double>("nz");
		}

		bool hasTextures = false;
		if (plyIn.getElement("vertex").hasProperty("t")) {
			hasTextures = true;
			ts = plyIn.getElement("vertex").getProperty<double>("t");
			ss = plyIn.getElement("vertex").getProperty<double>("s");
		}

		int TriangleCount = -1;
		for (int face = 0; face < fInd.size(); face++) {
			Vec3f V0, V1, V2, V3, N0, N1, N2, N3;
			Vec2f T0, T1, T2, T3;
			V0 = { (float)vPos[fInd[face][0]][0], (float)vPos[fInd[face][0]][1], (float)vPos[fInd[face][0]][2] };
			V1 = { (float)vPos[fInd[face][1]][0], (float)vPos[fInd[face][1]][1], (float)vPos[fInd[face][1]][2] };
			V2 = { (float)vPos[fInd[face][2]][0], (float)vPos[fInd[face][2]][1], (float)vPos[fInd[face][2]][2] };
			if (hasNormals) {
				N0 = { (float)nxs[fInd[face][0]], (float)nys[fInd[face][0]], (float)nzs[fInd[face][0]] };
				N1 = { (float)nxs[fInd[face][1]], (float)nys[fInd[face][1]], (float)nzs[fInd[face][1]] };
				N2 = { (float)nxs[fInd[face][2]], (float)nys[fInd[face][2]], (float)nzs[fInd[face][2]] };
			}

			if (hasTextures) {
				T0 = { (float)ss[fInd[face][0]], 1-(float)ts[fInd[face][0]]};
				T1 = { (float)ss[fInd[face][1]], 1-(float)ts[fInd[face][1]]};
				T2 = { (float)ss[fInd[face][2]], 1-(float)ts[fInd[face][2]]};
			}
			if (fInd[face].size() == 4) {
				V3 = { (float)vPos[fInd[face][3]][0], (float)vPos[fInd[face][3]][1], (float)vPos[fInd[face][3]][2] };
				if (hasNormals) N3 = { (float)nxs[fInd[face][3]], (float)nys[fInd[face][3]], (float)nzs[fInd[face][3]] };
				if (hasTextures) T3 = { (float)ss[fInd[face][3]], 1-(float)ts[fInd[face][3]] };
				faces.push_back((hasNormals) ? ((hasTextures) ? std::make_shared<Triangle>(V0, V2, V3, N0, N2, N3, T0, T2, T3) : std::make_shared<Triangle>(V0, V2, V3, N0, N2, N3)) : ((hasTextures) ? std::make_shared<Triangle>(V0, V2, V3, T0, T2, T3) : std::make_shared<Triangle>(V0, V2, V3)));
				TriangleCount += 1;
				ObjectBound = ObjectBound.Union(ObjectBound, faces[TriangleCount]->getWorldBounds());
				//std::cout << "face " << T0 <<" "<< T2 << " " << T3 << std::endl;
			}

			faces.push_back((hasNormals) ? ((hasTextures) ? std::make_shared<Triangle>(V0, V1, V2, N0, N1, N2, T0, T1, T2) : std::make_shared<Triangle>(V0, V1, V2, N0, N1, N2)) : ((hasTextures) ? std::make_shared<Triangle>(V0, V1, V2, T0, T1, T2) : std::make_shared<Triangle>(V0, V1, V2)));
			TriangleCount += 1;

			//std::cout << "face " << T0 << " " << T1 << " " << T2 << std::endl;
			
			ObjectBound = ObjectBound.Union(ObjectBound, faces[TriangleCount]->getWorldBounds());
		}

		VolumeRoot = BVH::BuildTree(faces);

		WorldBound = ObjectToWorld.TransformBounds3(ObjectBound);

	}

	Hit TriMesh::intersect(Ray ray) {

		//std::cout << "intersecting model" << std::endl;

		Ray r = WorldToObject.TransformRay(ray);

		Hit ret = VolumeRoot->Intersect(r);

		/*for (int i = 0; i < faces.size(); i++) {
			Hit t = faces[i]->intersect(r);
			if (t.valid && t.tValue < ret.tValue) {
				ret = t;
			}
		}*/
		
		ret.TrimeshTri = this;

		ret.tValue *= scale;

		if (ret.valid && (ret.TrimeshTri) == (ret.object)) {
			std::cout << "hit: " << ((Shape*)(ret.object))->name << std::endl;
			std::cout << "Tri: " << ((Shape*)(ret.TrimeshTri))->name << std::endl;
			std::cout << "problem " << ((Shape*)(ret.TrimeshTri))->name << " " << ((Shape*)(ret.object))->name << " " << name << std::endl;
		}
		ret.point = ObjectToWorld.TransformPoint(ret.point);

		return ret;
	}


} //namespace rt


