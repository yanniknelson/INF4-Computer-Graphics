/*
 * BVH.cpp
 *
 *
 */
#include "BVH.h"




namespace rt{


	BVH::Node* BVH::BuildTree(std::vector<std::shared_ptr<Shape>> shapes) {



		int numOfShapes = shapes.size();
		Node *ret = new Node();
		
		if (numOfShapes == 1) {
			ret->makeLeaf(shapes[0]);

		} else {

			Bounds3f shapeBound;
			for (int i = 0; i < numOfShapes; i++) {
				shapeBound = shapeBound.Union(shapeBound, shapes[i]->getWorldBounds());
			}

			Bounds3f centroidBounds;
			for (int i = 0; i < numOfShapes; i++) {
				centroidBounds = centroidBounds.Union(centroidBounds, shapes[i]->getCentroid());
			}

			int dim = centroidBounds.MaximumExtent();

			//sort the input shapes by their position in the chosen dimension
			std::sort(shapes.begin(), shapes.end(),
				[dim](const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) -> bool {
				return a->getCentroid()[dim] < b->getCentroid()[dim];
			});

            int mid = numOfShapes / 2;

			//if (numOfShapes <= 4) {
			//	/*std::nth_element(shapes[startIndex], shapes[mid], shapes[endIndex - 1] + 1, [dim](const Shape* a, const Shape* b) {
			//		return a->getCentroid()[dim] < b->getCentroid()[dim];
			//	});*/
			//}
			//else {
				constexpr int nBuckets = 12;
				struct BucketInfo {
					Bounds3f bounds;
					std::vector<std::shared_ptr<Shape>> shapes;
				};
				BucketInfo buckets[nBuckets];

				//for each shape, find its centroids position within the centroid bounds (from 0,0,0 to 1,1,1)
				//then add that shape to the that bucket's list of shapes and edit the bounds of the bucket to include the new shape
				for (int i = 0; i < numOfShapes; ++i) {
					int b = nBuckets * centroidBounds.Offset(shapes[i]->getCentroid())[dim];
					if (b == nBuckets) b = nBuckets - 1;
					buckets[b].shapes.push_back(shapes[i]);
					buckets[b].bounds = buckets[b].bounds.Union(buckets[b].bounds, shapes[i]->getWorldBounds());
				}

				//find the cost of all partitions of the buckets (there are n-1 partions for n buckets)
				float cost[nBuckets - 1];
				for (int i = 0; i < nBuckets - 1; ++i) {
					Bounds3f b0, b1;
					int count0 = 0, count1 = 0;
					for (int j = 0; j <= i; ++j) {
						if (buckets[j].shapes.size() > 0) {
							b0 = b0.Union(b0, buckets[j].bounds);
							count0 += buckets[j].shapes.size();
						}
					}
					for (int j = i + 1; j < nBuckets; ++j) {
						if (buckets[j].shapes.size() > 0) {
							b1 = b1.Union(b1, buckets[j].bounds);
							count1 += buckets[j].shapes.size();
						}
					}
					cost[i] = .125f + (count0 * b0.SurfaceArea() +
						count1 * b1.SurfaceArea()) / shapeBound.SurfaceArea();
				}

				//find the partition point with the lowest cost
				float minCost = cost[0];
				int minCostSplitBucket = 0;
				for (int i = 1; i < nBuckets - 1; ++i) {
					if (cost[i] < minCost) {
						minCost = cost[i];
						minCostSplitBucket = i;
					}
				}

				//the cost of making the current set a leaf 
				float leafCost = shapes.size();
				//if the min cost is less than the cost of making the current set a leaf, split using that partition
				//otherwise make the current set a leaf.
				if (minCost < leafCost) {
					std::vector<std::shared_ptr<Shape>> lowerPartition;
					std::vector<std::shared_ptr<Shape>> upperPartition;

					for (int i = 0; i <= minCostSplitBucket; i++) {
						lowerPartition.insert(lowerPartition.end(), buckets[i].shapes.begin(), buckets[i].shapes.end());
					}

					for (int i = minCostSplitBucket + 1; i < nBuckets; i++) {
						upperPartition.insert(upperPartition.end(), buckets[i].shapes.begin(), buckets[i].shapes.end());
					}
					ret->makeInterior(dim, shapes.size(), BuildTree(lowerPartition), BuildTree(upperPartition));
				} else {
					ret->makeLeaf(shapes, shapeBound);
				}

			//}
		}
		return ret;
	}


} //namespace rt


