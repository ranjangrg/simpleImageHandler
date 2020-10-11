#ifndef __IMAGE_HANDLER_HPP
#define __IMAGE_HANDLER_HPP

#include <iostream>
#include <vector>
#include <iomanip>	// for std::cout and format
#include <memory>	// for unique_ptr

#include "./stb_image.h"
#include "./stb_image_write.h"

namespace ImageHandler {
	class Image {
		public:
		size_t width, height, nChannels;
	};

	// a struct that holds image data including number of channels and actual pixel data per channel
	class ImageUC {
		public:
		size_t width, height, nChannels;
		unsigned char *imgData;

		ImageUC();
		ImageUC(size_t width, size_t height, size_t nChannels);

		void infoAll();	// prints all channel(s) info
		void infoChannel(size_t channelIdx);	// only prints specified channel
	};

	class ImageV {
		public:
		size_t width, height, nChannels;
		std::vector<unsigned char> data;
	};
}

namespace ImageHandler {
	std::unique_ptr<ImageHandler::ImageUC> getImageDataAsUC(const char* fileName);
	std::unique_ptr<ImageHandler::ImageV> getImageDataAsV(const char* fileName);
}

#endif
