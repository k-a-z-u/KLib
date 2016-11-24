#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/ocr/OCRSegmentation.h"
#include "../../../cv/ImageFactory.h"

using namespace K;

#ifdef WITH_JPG

TEST(OCR, segmentation) {

	ImageChannel img = ImageFactory::readJPEG(getDataFile("text_scan.jpg"));

	OCRSegmentation::getLetters(img);

}

#endif

#endif
