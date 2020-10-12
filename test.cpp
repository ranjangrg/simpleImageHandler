#include "./test.hpp"

void testImageStruct() {
	//std::unique_ptr<ImageHandler::ImageUC> imgPtr =	ImageHandler::getImageDataAsImageUC("./data/uneven.png");
	//imgPtr->infoAll();
	//imgPtr->infoChannel(0);

	std::unique_ptr<ImageHandler::ImageV> imgPtr =	ImageHandler::getImageDataAsImageV("./data/uneven.png");

	imgPtr->infoAll();
	//imgPtr->infoChannel(0);
}

int main() {
	std::cout << "Started ..." << std::endl;

	testImageStruct();

	return 0;
}