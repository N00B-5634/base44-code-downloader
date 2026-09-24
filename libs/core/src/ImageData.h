#ifndef YELLOWDOCS_CORE_IMAGE_DATA_H
#define YELLOWDOCS_CORE_IMAGE_DATA_H

#include <SupportDefs.h>
#include <String.h>
#include <DataIO.h>

namespace YellowDocs {
namespace Core {

class ImageData {
public:
	ImageData();
	ImageData(const ImageData& other);
	ImageData& operator=(const ImageData& other);
	~ImageData();

	// Create from file
	status_t LoadFromFile(const BString& path);
	status_t SaveToFile(const BString& path) const;

	// Create from data
	status_t LoadFromData(const uint8* data, size_t size);
	status_t GetData(uint8** data, size_t* size) const;

	// Properties
	void SetFilePath(const BString& path);
	BString FilePath() const;

	void SetWidth(float width);
	float Width() const;

	void SetHeight(float height);
	float Height() const;

	void SetNaturalWidth(int width);
	int NaturalWidth() const;

	void SetNaturalHeight(int height);
	int NaturalHeight() const;

	void SetScale(float scale);
	float Scale() const;

	void SetAltText(const BString& text);
	BString AltText() const;

	// Image type
	void SetImageType(const BString& type);
	BString ImageType() const;

	// Check if valid
	bool IsValid() const;

	// Clear data
	void Clear();

private:
	BString fFilePath;
	float fWidth;
	float fHeight;
	int fNaturalWidth;
	int fNaturalHeight;
	float fScale;
	BString fAltText;
	BString fImageType;
	BDataIO* fImageData;
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_IMAGE_DATA_H
