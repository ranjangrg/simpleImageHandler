#include "./test.hpp"

void testImageStruct() {
	// using ImageUC object
	std::unique_ptr<ImageHandler::ImageUC> imgPtrUC =	ImageHandler::getImageDataAsImageUC("./data/pixel.png");
	ImageHandler::modifyData(imgPtrUC);
	ImageHandler::writeImageUCDataToFile(imgPtrUC, "./data/delme1.png");

	// using ImageV object
	std::unique_ptr<ImageHandler::ImageV> imgPtrV =	ImageHandler::getImageDataAsImageV("./data/uneven.png");
	ImageHandler::modifyData(imgPtrV);
	ImageHandler::writeImageVDataToFile(imgPtrV, "./data/delme2.png");
}

int main() {
	//std::cout << "Started ..." << std::endl;

	testImageStruct();

	return 0;
}