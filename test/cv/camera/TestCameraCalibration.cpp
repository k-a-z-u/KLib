#ifndef TESTLENSCALIBRATION_CPP
#define TESTLENSCALIBRATION_CPP

#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/camera/CameraIntrinsics.h"

namespace K {



	TEST(CameraCalibration, calibrate) {

//		std::vector<CameraIntrinsics::Pair> pairs;

//		pairs.push_back( CameraIntrinsics::Pair(1,1,0,	168,179) );
//		pairs.push_back( CameraIntrinsics::Pair(2,1,0,	190,180) );
//		pairs.push_back( CameraIntrinsics::Pair(3,1,0,	213,181) );
//		pairs.push_back( CameraIntrinsics::Pair(4,1,0,	236,182) );
//		pairs.push_back( CameraIntrinsics::Pair(5,1,0,	259,184) );
//		pairs.push_back( CameraIntrinsics::Pair(6,1,0,	281,185) );
//		pairs.push_back( CameraIntrinsics::Pair(7,1,0,	304,187) );
//		pairs.push_back( CameraIntrinsics::Pair(8,1,0,	326,188) );
//		pairs.push_back( CameraIntrinsics::Pair(9,1,0,	348,190) );
//		pairs.push_back( CameraIntrinsics::Pair(10,1,0,	370,191) );
//		pairs.push_back( CameraIntrinsics::Pair(11,1,0,	391,192) );
//		pairs.push_back( CameraIntrinsics::Pair(12,1,0,	413,194) );


//		pairs.push_back( CameraIntrinsics::Pair(1,12,0,	132,377) );
//		pairs.push_back( CameraIntrinsics::Pair(2,12,0,	164,378) );
//		pairs.push_back( CameraIntrinsics::Pair(3,12,0,	197,379) );
//		pairs.push_back( CameraIntrinsics::Pair(4,12,0,	228,382) );
//		pairs.push_back( CameraIntrinsics::Pair(5,12,0,	260,382) );
//		pairs.push_back( CameraIntrinsics::Pair(6,12,0,	292,383) );

//		pairs.push_back( CameraIntrinsics::Pair(7,12,0,		323,384) );
//		pairs.push_back( CameraIntrinsics::Pair(8,12,0,		354,385) );
//		pairs.push_back( CameraIntrinsics::Pair(9,12,0,		385,385) );
//		pairs.push_back( CameraIntrinsics::Pair(10,12,0,	414,386) );
//		pairs.push_back( CameraIntrinsics::Pair(11,12,0,	443,386) );
//		pairs.push_back( CameraIntrinsics::Pair(12,12,0,	472,386) );

		CameraIntrinsics::Plane img1;
		img1.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	167,179));
		img1.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	434,195));
		img1.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	499,386));
		img1.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	132,377));

		CameraIntrinsics::Plane img2;
		img2.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	281,20));
		img2.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	542,90));
		img2.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	460,341));
		img2.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	235,243));

		CameraIntrinsics::Plane img3;
		img3.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	167,30));
		img3.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	486,76));
		img3.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	424,318));
		img3.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	156,281));

		CameraIntrinsics::Plane img4;
		img4.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	163,55));
		img4.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	527,104));
		img4.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	452,369));
		img4.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	152,339));

		CameraIntrinsics::Plane img5;
		img5.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	147,75));
		img5.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	538,116));
		img5.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	461,395));
		img5.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	142,385));

		CameraIntrinsics::Plane img6;
		img6.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	142,65));
		img6.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	567,106));
		img6.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	486,411));
		img6.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	140,409));

		CameraIntrinsics::Plane img7;
		img7.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	42,67));
		img7.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	477,104));
		img7.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	427,446));
		img7.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	45,452));

		CameraIntrinsics::Plane img8;
		img8.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	287,84));
		img8.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	604,158));
		img8.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	562,458));
		img8.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	217,392));

		CameraIntrinsics::Plane img9;
		img9.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	128,63));
		img9.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	604,166));
		img9.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	479,457));
		img9.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	142,362));

		CameraIntrinsics::Plane img10;
		img10.pairs.push_back( CameraIntrinsics::Pair(0,0,0,	46,22));
		img10.pairs.push_back( CameraIntrinsics::Pair(1,0,0,	551,198));
		img10.pairs.push_back( CameraIntrinsics::Pair(1,1,0,	450,450));
		img10.pairs.push_back( CameraIntrinsics::Pair(0,1,0,	137,292));

		std::vector<CameraIntrinsics::Plane> planes;
		planes.push_back(img1);
		planes.push_back(img2);
		planes.push_back(img3);
		planes.push_back(img4);
		planes.push_back(img5);
		planes.push_back(img6);
		planes.push_back(img7);
		planes.push_back(img8);
		planes.push_back(img9);
		planes.push_back(img10);

		CameraIntrinsics::calibrate<10>(planes);


	}

}

#endif


#endif // TESTLENSCALIBRATION_CPP
