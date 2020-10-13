#include "./test.hpp"

void testImageStruct() {
	std::unique_ptr<ImageHandler::ImageUC> imgPtrUC =	ImageHandler::getImageDataAsImageUC("./data/uneven.png");
	ImageHandler::writeImageUCDataToFile(imgPtrUC, "./data/delme1.png");

	std::unique_ptr<ImageHandler::ImageV> imgPtrV =	ImageHandler::getImageDataAsImageV("./data/uneven.png");
	ImageHandler::writeImageVDataToFile(imgPtrV, "./data/delme2.png");
}

int main() {
	std::cout << "Started ..." << std::endl;

	testImageStruct();

	return 0;
}