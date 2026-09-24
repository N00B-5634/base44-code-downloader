#include "ImageData.h"

#include <File.h>
#include <Path.h>
#include <new>

namespace YellowDocs {
namespace Core {

ImageData::ImageData()
	: fWidth(0),
	  fHeight(0),
	  fNaturalWidth(0),
	  fNaturalHeight(0),
	  fScale(1.0f),
	  fImageData(nullptr)
{
}

ImageData::ImageData(const ImageData& other)
	: fFilePath(other.fFilePath),
	  fWidth(other.fWidth),
	  fHeight(other.fHeight),
	  fNaturalWidth(other.fNaturalWidth),
	  fNaturalHeight(other.fNaturalHeight),
	  fScale(other.fScale),
	  fAltText(other.fAltText),
	  fImageType(other.fImageType),
	  fImageData(nullptr)
{
	if (other.fImageData) {
		fImageData = new BDataIO(other.fImageData);
	}
}

ImageData& ImageData::operator=(const ImageData& other) {
	if (this != &other) {
		fFilePath = other.fFilePath;
		fWidth = other.fWidth;
		fHeight = other.fHeight;
		fNaturalWidth = other.fNaturalWidth;
		fNaturalHeight = other.fNaturalHeight;
		fScale = other.fScale;
		fAltText = other.fAltText;
		fImageType = other.fImageType;
		
		delete fImageData;
		fImageData = nullptr;
		
		if (other.fImageData) {
			fImageData = new BDataIO(other.fImageData);
		}
	}
	return *this;
}

ImageData::~ImageData() {
	Clear();
}

// Create from file

status_t ImageData::LoadFromFile(const BString& path) {
	Clear();
	
	BFile file(path.String(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	fFilePath = path;
	
	// Read file data
	off_t size;
	file.GetSize(&size);
	
	if (size > 0) {
		fImageData = new BDataIO();
		uint8* buffer = new uint8[size];
		ssize_t bytesRead = file.Read(buffer, size);
		
		if (bytesRead == size) {
			fImageData->Write(buffer, size);
			fImageData->Seek(0, SEEK_SET);
		}
		
		delete[] buffer;
		
		if (bytesRead != size) {
			delete fImageData;
			fImageData = nullptr;
			return B_IO_ERROR;
		}
	}
	
	// Set default dimensions based on image type
	BPath filePath(path.String());
	BString extension = filePath.PathExtension();
	extension.ToLower();
	
	fImageType = extension;
	fNaturalWidth = 0;
	fNaturalHeight = 0;
	fWidth = 100; // Default width
	fHeight = 100; // Default height
	
	return B_OK;
}

status_t ImageData::SaveToFile(const BString& path) const {
	if (!fImageData || !IsValid()) {
		return B_BAD_VALUE;
	}
	
	BFile file(path.String(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	fImageData->Seek(0, SEEK_SET);
	uint8 buffer[4096];
	ssize_t bytesRead;
	
	while ((bytesRead = fImageData->Read(buffer, sizeof(buffer))) > 0) {
		file.Write(buffer, bytesRead);
	}
	
	return file.InitCheck();
}

// Create from data

status_t ImageData::LoadFromData(const uint8* data, size_t size) {
	Clear();
	
	if (!data || size == 0) {
		return B_BAD_VALUE;
	}
	
	fImageData = new BDataIO();
	fImageData->Write(data, size);
	fImageData->Seek(0, SEEK_SET);
	
	fWidth = 100;
	fHeight = 100;
	
	return B_OK;
}

status_t ImageData::GetData(uint8** data, size_t* size) const {
	if (!fImageData || !data || !size) {
		return B_BAD_VALUE;
	}
	
	fImageData->Seek(0, SEEK_END);
	size_t dataSize = fImageData->Position();
	fImageData->Seek(0, SEEK_SET);
	
	*size = dataSize;
	*data = new uint8[dataSize];
	
	fImageData->Read(*data, dataSize);
	
	return B_OK;
}

// Properties

void ImageData::SetFilePath(const BString& path) {
	fFilePath = path;
}

BString ImageData::FilePath() const {
	return fFilePath;
}

void ImageData::SetWidth(float width) {
	fWidth = width;
}

float ImageData::Width() const {
	return fWidth;
}

void ImageData::SetHeight(float height) {
	fHeight = height;
}

float ImageData::Height() const {
	return fHeight;
}

void ImageData::SetNaturalWidth(int width) {
	fNaturalWidth = width;
}

int ImageData::NaturalWidth() const {
	return fNaturalWidth;
}

void ImageData::SetNaturalHeight(int height) {
	fNaturalHeight = height;
}

int ImageData::NaturalHeight() const {
	return fNaturalHeight;
}

void ImageData::SetScale(float scale) {
	fScale = scale;
}

float ImageData::Scale() const {
	return fScale;
}

void ImageData::SetAltText(const BString& text) {
	fAltText = text;
}

BString ImageData::AltText() const {
	return fAltText;
}

// Image type

void ImageData::SetImageType(const BString& type) {
	fImageType = type;
}

BString ImageData::ImageType() const {
	return fImageType;
}

// Check if valid

bool ImageData::IsValid() const {
	return fImageData != nullptr;
}

// Clear data

void ImageData::Clear() {
	fFilePath = "";
	fWidth = 0;
	fHeight = 0;
	fNaturalWidth = 0;
	fNaturalHeight = 0;
	fScale = 1.0f;
	fAltText = "";
	fImageType = "";
	
	delete fImageData;
	fImageData = nullptr;
}

} // namespace Core
} // namespace YellowDocs
