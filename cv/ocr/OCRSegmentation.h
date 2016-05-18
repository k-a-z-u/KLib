#ifndef K_CV_OCR_OCRSEGMENTATION_H
#define K_CV_OCR_OCRSEGMENTATION_H

#include "../ImageChannel.h"
#include "../ImageFactory.h"
#include "../draw/Drawer.h"

#include "../filter/Threshold.h"
#include "../segmentation/Segmentation.h"
#include "../../data/kd-tree/KDTree.h"
#include "../../data/kd-tree/KDTreeKNN.h"


namespace K {

	class OCRSegmentation {

	private:

		class Letters {
		public:
			std::vector<Segment> segments;
		public:
			Letters(std::vector<Segment>& segments) : segments(segments) {;}
			inline int kdGetValue(const KDIdx idx, const int axis) const {
				//return segments[idx].calcBBox().getCenter()[axis];
				return 0;// TODO
			}
			inline int kdGetDistance(const KDIdx idx, const int* values) const {
				const Point2i p1(values[0], values[1]);
				const Point2i p2 = segments[idx].calcBBox().getCenter();
				return p1.getDistance(p2);
			}
		};

	public:

		static void getLetters(const ImageChannel& img) {

			// get all segments
			std::vector<Segment> segments = getSegments(img);
			Letters l(segments);

			// nearest neighbor search tree
			using CFG = KDTreeConfig<int, Letters, 2, KDTreeSplit::AVG>;
			KDTree<CFG> tree;
			tree.setDataSource(&l);
			tree.addAll((KDIdx)l.segments.size());

			ImageChannel img2(img.getData(), img.getWidth(), img.getHeight());
			Drawer d(img2);

			// process each segment
			int j = 0;
			for (int s = 0; s < segments.size(); ++s) {

				const Segment seg = segments[s];
				BBox2i bb = seg.calcBBox();
				std::vector<KDTreeNeighbor<int>> neighbors = KDTreeKNN::getNeighborsWithinRadius(tree, {bb.getCenter().x, bb.getMin().y}, bb.getHeight());

				if (neighbors.size() > 1) {

					bb.growValue(3);
					bb.add( Point2i(bb.getMin().x, bb.getMin().y - bb.getHeight() / 2) );

					for (KDTreeNeighbor<int> nn : neighbors) {
						if (nn.idx == s) {continue;}		// skip the segment itself
						Segment& seg2 = segments[nn.idx];
						const BBox2i bb2 = seg2.calcBBox();
						if (bb.contains(bb2)) {
							d.drawRect(bb.getMin(), bb.getMax());
							d.drawRect(bb2.getMin(), bb2.getMax());
						}
					}
				}
			}

			ImageFactory::writePNG("/tmp/segs.png", img2);

			// try to combine segments (o -> ö)


//			ImageChannel img2(img.getWidth(), img.getHeight());
//			img2.ones();
//			Drawer d(img2);
//			for (Segment& s : segs) {
//				const BBox2i bb = s.calcBBox();
//				d.drawRect(bb.getMin(), bb.getMax());
//			}
//			ImageFactory::writePNG("/tmp/segs.png", img2);

		}

		// try to segmentize all connected regions within the image
		static std::vector<Segment> getSegments(const ImageChannel& img) {

			// apply threshold?
			//Threshold:(img, 0.80f);

			// mark all (nearly) white pixels as "already segmentized" -> ignore them
			Bitmap used(img, 0.8f);

			// get all segments -> connected black regions -> letters
			return Segmentation::getSegments(img, used, 0.99f);

		}

	};


}


#endif // K_CV_OCR_OCRSEGMENTATION_H
