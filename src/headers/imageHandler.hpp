#ifndef __IMAGE_HANDLER_HPP
#define __IMAGE_HANDLER_HPP

#include <iostream>
#include <vector>
#include <iomanip>	// for std::cout and format
#include <memory>	// for unique_ptr

namespace ImageHandler {
	class Image {
		public:
		size_t width, height, nChannels;

		Image();
		Image(size_t width, size_t height, size_t nChannels);

		virtual void infoAll();	// prints all channel(s) info
		virtual void infoChannel(size_t channelIdx);	// only prints specified channel
	};

	class ImageUC : public Image{
		public:
		unsigned char *imgData;

		ImageUC();
		ImageUC(size_t width, size_t height, size_t nChannels);
		~ImageUC();

		void infoAll() override;	// prints all channel(s) info
		void infoChannel(size_t channelIdx) override;	// only prints specified channel
	};

	class ImageV : public Image {
		public:
		std::vector<unsigned char> imgData;

		ImageV();
		ImageV(size_t width, size_t height, size_t nChannels);

		void infoAll() override;	// prints all channel(s) info
		void infoChannel(size_t channelIdx) override;	// only prints specified channel
	};
}

namespace ImageHandler {
	std::unique_ptr<ImageHandler::ImageUC> getImageDataAsImageUC(const char* fileName);
	std::unique_ptr<ImageHandler::ImageV> getImageDataAsImageV(const char* fileName);
}

#endif
