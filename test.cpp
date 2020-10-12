#include "./test.hpp"

void testImageStruct() {
	//std::unique_ptr<ImageHandler::ImageUC> imgPtr =	ImageHandler::getImageDataAsImageUC("./data/uneven.png");
	//imgPtr->infoAll();
	//imgPtr->infoChannel(0);

	std::unique_ptr<ImageHandler::ImageV> imgPtr =	ImageHandler::getImageDataAsImageV("./data/uneven.png");

	//imgPtr->infoAll();
	//imgPtr->infoChannel(0);

	std::unique_ptr<unsigned char> ucData = ImageHandler::imageVToUC(imgPtr);
	//ImageHandler::_printUC(ucData.get(), imgPtr->width, imgPtr->height, imgPtr->nChannels);

	//ImageHandler::_printUC(imgPtr->imgData, imgPtr->width, imgPtr->height, imgPtr->nChannels);

	ImageHandler::writeImageDataToFile(imgPtr, "./data/delme.png");
}

int main() {
	std::cout << "Started ..." << std::endl;

	testImageStruct();

	return 0;
}