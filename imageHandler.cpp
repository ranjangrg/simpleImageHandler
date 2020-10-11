#include "./imageHandler.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include "./stb_image.h"
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include "./stb_image_write.h"
#endif

ImageHandler::ImageUC::ImageUC() {
	this->width = 0;
	this->height = 0;
	this->nChannels = 0;
	this->imgData = new unsigned char[0];
}

ImageHandler::ImageUC::ImageUC(size_t width, size_t height, size_t nChannels) {
	this->width = width;
	this->height = height;
	this->nChannels = nChannels;
	this->imgData = new unsigned char[0];
}

void ImageHandler::ImageUC::infoAll() {
	size_t currChannel = 0;
	std::cout << "Info:" << std::endl;
	std::cout << "Dimensions: " << width << " x " << height << std::endl;
	for (size_t currChannel = 0; currChannel < this->nChannels; ++currChannel) {
		std::cout << "Channel: " << currChannel << std::endl;
		for (size_t rowIdx = 0; rowIdx < this->height; ++rowIdx) {
			for (size_t colIdx = 0; colIdx < this->width; ++colIdx) {
				unsigned char* currPixel = this->imgData + ( (rowIdx * this->width + colIdx) * this->nChannels );
				std::cout << std::setw(4) << int(currPixel[currChannel]) << ' ';				
			}
			std::cout << std::endl;
		}
	}
}

void ImageHandler::ImageUC::infoChannel(size_t channelIdx) {
	std::cout << "Channel: " << channelIdx << std::endl;
	for (size_t rowIdx = 0; rowIdx < this->height; ++rowIdx) {
		for (size_t colIdx = 0; colIdx < this->width; ++colIdx) {
			unsigned char* currPixel = this->imgData + ( (rowIdx * this->width + colIdx) * this->nChannels );
			std::cout << std::setw(4) << int(currPixel[channelIdx]) << ' ';			
		}
		std::cout << std::endl;
	}
}

std::unique_ptr<ImageHandler::ImageUC> ImageHandler::getImageDataAsUC(const char* fileName) {
	int width, height, nChannels;
	unsigned char * imgData = stbi_load(fileName, &width, &height, &nChannels, 0);

	if(stbi_failure_reason()) {
		//std::cout << "Failure: " << stbi_failure_reason() << std::endl;
	}

	std::string errMsg, infoMsg;
	if (imgData == NULL) {
		errMsg = "Error loading image: " + std::string(fileName) + '\n';
	} else {
		infoMsg = "Loaded file: '" + std::string(fileName) + "'; ";
		infoMsg +=  "Dimensions: (" + std::to_string(width) + ", " + std::to_string(height) + "); ";
		infoMsg +=  "Channels: " + std::to_string(nChannels);
	}
	std::cout << errMsg << infoMsg << std::endl;

	std::unique_ptr<ImageHandler::ImageUC> imgPtr = std::make_unique<ImageHandler::ImageUC>(width, height, nChannels);
	imgPtr->imgData = imgData;
	return std::move(imgPtr);
}

