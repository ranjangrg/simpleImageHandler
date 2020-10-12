#include "./imageHandler.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include "./stb_image.h"
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include "./stb_image_write.h"
#endif

//	======================
//	Image class definition 
//	====================== 
ImageHandler::Image::Image() {
	this->width = 0;
	this->height = 0;
	this->nChannels = 0;
}

ImageHandler::Image::Image(size_t width, size_t height, size_t nChannels) {
	this->width = width;
	this->height = height;
	this->nChannels = nChannels;
}

void ImageHandler::Image::infoAll() {
	size_t currChannel = 0;
	std::cout << "Info:" << std::endl;
	std::cout << "Dimensions: " << width << " x " << height << std::endl;
}

void ImageHandler::Image::infoChannel(size_t channelIdx) {
	std::cout << "Channel: " << channelIdx << std::endl;
}

//	========================
//	ImageUC class definition 
//	======================== 
ImageHandler::ImageUC::ImageUC() : ImageHandler::Image::Image() {
	this->imgData = new unsigned char[0];
}

ImageHandler::ImageUC::ImageUC(size_t width, size_t height, size_t nChannels) : 
	ImageHandler::Image::Image(width, height, nChannels) {
	this->imgData = new unsigned char[width * height * nChannels];
}

ImageHandler::ImageUC::~ImageUC() {
	//std::cout << "Deleting ImageUC\n";
	delete this->imgData;
}

void ImageHandler::ImageUC::infoAll() {
	ImageHandler::Image::infoAll();
	size_t currChannel = 0;
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
	ImageHandler::Image::infoChannel(channelIdx);
	for (size_t rowIdx = 0; rowIdx < this->height; ++rowIdx) {
		for (size_t colIdx = 0; colIdx < this->width; ++colIdx) {
			unsigned char* currPixel = this->imgData + ( (rowIdx * this->width + colIdx) * this->nChannels );
			std::cout << std::setw(4) << int(currPixel[channelIdx]) << ' ';			
		}
		std::cout << std::endl;
	}
}

//	======================= 
//	ImageV class definition 
//	======================= 
ImageHandler::ImageV::ImageV() : ImageHandler::Image::Image() {
	this->imgData.reserve(0);
}

ImageHandler::ImageV::ImageV(size_t width, size_t height, size_t nChannels) : 
	ImageHandler::Image::Image(width, height, nChannels) {
	this->imgData.reserve(width * height * nChannels);
}

void ImageHandler::ImageV::infoAll() {
	ImageHandler::Image::infoAll();
	size_t currChannel = 0;
	size_t imageRes = width * height;

	for (size_t currChannel = 0; currChannel < this->nChannels; ++currChannel) {
		std::cout << "Channel: " << currChannel << std::endl;
		for (size_t rowIdx = 0; rowIdx < this->height; ++rowIdx) {
			for (size_t colIdx = 0; colIdx < this->width; ++colIdx) {
				unsigned char currData = this->imgData.at( (rowIdx * this->width + colIdx) + (imageRes * currChannel) );
				std::cout << std::setw(4) << int(currData) << ' ';				
			}
			std::cout << std::endl;
		}
	}
}

void ImageHandler::ImageV::infoChannel(size_t channelIdx) {
	ImageHandler::Image::infoChannel(channelIdx);
	size_t imageRes = width * height;
	for (size_t rowIdx = 0; rowIdx < this->height; ++rowIdx) {
		for (size_t colIdx = 0; colIdx < this->width; ++colIdx) {
			unsigned char currData = this->imgData.at( (rowIdx * this->width + colIdx) + (imageRes * channelIdx) );
			std::cout << std::setw(4) << int(currData) << ' ';			
		}
		std::cout << std::endl;
	}
}


//	===============================
//	ImageHandler method definitions
//	===============================
std::unique_ptr<ImageHandler::ImageUC> ImageHandler::getImageDataAsImageUC(const char* fileName) {
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

std::unique_ptr<ImageHandler::ImageV> ImageHandler::getImageDataAsImageV(const char* fileName) {
	std::unique_ptr<ImageHandler::ImageUC> imgUC = ImageHandler::getImageDataAsImageUC(fileName);
	unsigned char * imgData = imgUC->imgData;
	int width = imgUC->width;
	int height = imgUC->height;
	int nChannels = imgUC->nChannels;

	std::unique_ptr<ImageHandler::ImageV> imgPtr = std::make_unique<ImageHandler::ImageV>(width, height, nChannels);
	size_t imageRes = width * height;
	for (size_t channelIdx = 0; channelIdx < nChannels; ++channelIdx) {
		for (size_t rowIdx = 0; rowIdx < imgPtr->height; ++rowIdx) {
			for (size_t colIdx = 0; colIdx < imgPtr->width; ++colIdx) {
				unsigned char* currPixel = imgUC->imgData + ( (rowIdx * imgPtr->width + colIdx) * nChannels );
				imgPtr->imgData.push_back(currPixel[channelIdx]);
			}
		}
	}

	std::cout << "Channel count: " << nChannels << std::endl;

	return std::move(imgPtr);
}

