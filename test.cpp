#include "./test.hpp"

void testImageStruct() {
	//auto imgPtr = ImageHandler::createImageUC(10, 10);
	//ImageHandler::ImageUC imgPtr = ImageHandler::ImageUC(10, 20, 3);
	std::unique_ptr<ImageHandler::ImageUC> imgPtr =	ImageHandler::getImageDataAsUC("./data/uneven.png");

	imgPtr->infoAll();
}

int main() {
	std::cout << "Started ..." << std::endl;

	testImageStruct();

	return 0;
}