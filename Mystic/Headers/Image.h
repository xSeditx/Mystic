#pragma once
#include "SOIL.h"
#include "Asset.h" /// Perhaps inside of  CPP would be better

#pragma comment(lib , "debugSOIL.lib") // SOIL Lib I built for _x64 debug
 
class Image
{
public:
	Image() = default;	~Image();

	Image(std::string file);
	Image(void *_data, Vec2 _dims, unsigned int _channels);

	unsigned char *Data = nullptr;
	std::string Filepath;
	int Channels = 4;

	iVec2 Size = iVec2(0);
    size_t DataSize = 0;
	bool Reload();

	const unsigned int size() { return (Size.x * Size.y * (sizeof(unsigned char) * Channels)); }

    static Image FlippedImage(Image &_image);
};

