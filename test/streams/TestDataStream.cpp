/*
 * TestDataStream.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: kazu
 */




#ifdef WITH_TESTS
#include "../Test.h"
#include "../../klib/streams/DataOutputStream.h"
#include "../../klib/streams/DataInputStream.h"
#include "../../klib/streams/ByteArrayInOutStream.h"

using namespace K;



TEST(DataStream, readWrite) {

	ByteArrayInOutStream baios;
	DataOutputStream dos(baios);
	DataInputStream dis(baios);

	dos.write(8);
	dos.writeShort(1337);
	dos.writeInt(133713337);
	dos.writeLong(1337133713371337);
	dos.writeFloat(1.337f);
	dos.writeDouble(1.33713371337);

	ASSERT_EQ(8, dis.read());
	ASSERT_EQ(1337, dis.readShort());
	ASSERT_EQ(133713337, dis.readInt());
	ASSERT_EQ(1337133713371337, dis.readLong());
	ASSERT_EQ(1.337f, dis.readFloat());
	ASSERT_EQ(1.33713371337, dis.readDouble());

}



#endif
