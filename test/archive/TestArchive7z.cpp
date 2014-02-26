/*
 * TestArchive7z.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: kazu
 */


#ifdef WITH_TESTS
#ifdef WITH_7Z

#include "../Test.h"
#include "../../archive/Archive7z.h"
#include <vector>
#include <cstdint>

using namespace K;

TEST(Archive7z, open) {

	std::string s1 = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n";
	std::string s2 = "Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. \n";
	std::string s3 = "Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. \n";

	File folder = File(__FILE__).getParent();
	File file = File(folder, "test.7z");
	Archive7z zz(file);

	Archive7zComposite root = zz.getFiles();

	ASSERT_EQ(3, root.getChilds().size());

	ASSERT_EQ("1.txt", root.getChilds().at(0).getName());
	ASSERT_EQ("2.txt", root.getChilds().at(1).getName());
	ASSERT_EQ("3", root.getChilds().at(2).getName());
	ASSERT_EQ("3.txt", root.getChilds().at(2).getChilds().at(0).getName());

	std::vector<uint8_t> ret;
	root.getChilds().at(0).decompress(ret);
	ASSERT_EQ(s1, std::string((char*)&ret[0], ret.size()));

	root.getChilds().at(1).decompress(ret);
	ASSERT_EQ(s2, std::string((char*)&ret[0], ret.size()));

	root.getChilds().at(2).getChilds().at(0).decompress(ret);
	ASSERT_EQ(s3, std::string((char*)&ret[0], ret.size()));

}

#endif
#endif
