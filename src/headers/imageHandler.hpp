#ifndef __IMAGE_HANDLER_HPP
#define __IMAGE_HANDLER_HPP

#include <iostream>
#include <vector>
#include <iomanip>	// for std::cout and format
#include <memory>	// for unique_ptr, shared_ptr etc

#include "./matrix.hpp"	// for matrix implementation

namespace ImageHandler {
	class Image {
		public:
		size_t width, height, nChannels;

		Image();
		Image(size_t width, size_t height, size_t nChannels);

		virtual void infoAll();	// prints all channel(s) info
		virtual void infoChannel(size_t channelIdx);	// only prints specified channel
	};

	// image data stored as unsigned char
	class ImageUC : public Image{
		public:
		unsigned char *imgData;

		ImageUC();
		ImageUC(size_t width, size_t height, size_t nChannels);
		~ImageUC();

		void infoAll() override;	// prints all channel(s) info
		void infoChannel(size_t channelIdx) override;	// only prints specified channel
	};

	// image data stored as vector
	class ImageV : public Image {
		public:
		std::vector<unsigned char> imgData;	// each elem stores a whole channel

		ImageV();
		ImageV(size_t width, size_t height, size_t nChannels);
		~ImageV();

		void infoAll() override;	// prints all channel(s) info
		void infoChannel(size_t channelIdx) override;	// only prints specified channel
	};

	// image data stored as MatrixStruct
	class ImageM : public Image {
		public:
		std::vector<Matrix::MatrixStruct<unsigned char>> imgData;

		ImageM();
		ImageM(size_t width, size_t height, size_t nChannels);
		~ImageM();

		void infoAll() override;	// prints all channel(s) info
		void infoChannel(size_t channelIdx) override;	// only prints specified channel
	};

}

namespace ImageHandler {
	std::unique_ptr<ImageUC> getImageDataAsImageUC(const char* fileName);
	std::unique_ptr<ImageV> getImageDataAsImageV(const char* fileName);
	std::unique_ptr<ImageM> getImageDataAsImageM(const char* fileName);

	unsigned char* imageVToUC(std::unique_ptr<ImageV>& imageDataPtr);
	unsigned char* imageMToUC(std::unique_ptr<ImageM>& imageDataPtr);

	// just for debugging; can be deleted once prod version is established
	void _printUC(unsigned char* imgData, size_t width, size_t height, size_t nChannels);

	// debug only
	// just puts RGB pixel in the middle of image (4 channels accepted)
	void modifyData(std::unique_ptr<ImageUC>& imageDataPtr);
	void modifyData(std::unique_ptr<ImageV>& imageDataPtr);
	void modifyData(std::unique_ptr<ImageM>& imageDataPtr);
	void modifyDataConvolute(std::unique_ptr<ImageM>& imageDataPtr);

	int writeRawImageDataToFile(const char* fileToWrite, size_t width, size_t height, size_t nChannels, unsigned char* imageData);
	int writeImageUCDataToFile(std::unique_ptr<ImageUC>& imageDataPtr, const char* fileToWrite);
	int writeImageVDataToFile(std::unique_ptr<ImageV>& imageDataPtr, const char* fileToWrite);
	int writeImageMDataToFile(std::unique_ptr<ImageM>& imageDataPtr, const char* fileToWrite);
}

#endif
