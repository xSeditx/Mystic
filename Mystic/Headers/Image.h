#pragma once
#include "SOIL.h"
#include "Asset.h" /// Perhaps inside of  CPP would be better

#pragma comment(lib , "debugSOIL.lib") // SOIL Lib I built for _x64 debug
 
class Bitmap
{
public:
	Bitmap() = default;	~Bitmap();

	/* Create a Bitmap from an Image File */
	Bitmap(std::string file);

	/* Create a Bitmap from Data in Memory */
	Bitmap(void *_data, Vec2 _dims, unsigned int _channels);

	/* Going to be used for Hot reloading of the Bitmap image*/
	bool Reload();

	/* return the size in Bytes of the Bitmap */
	const unsigned int size() { return (Size.x * Size.y * (sizeof(unsigned char) * Channels)); }

	/* Return the Width of the Bitmap */
	inline uint32_t Width() { return Size.x; }

	/* Returns the Height  */
	inline uint32_t Height() { return Size.y; }

	/* returns the number of RGBA components avalible in the Bitmap */
	int BytesPerPixel() { return Channels; }

	/* Utility function that returns an Image with Inverted Y component
	NOTE: I believe their might currently be an Error here */
    static Bitmap& FlippedImage(Bitmap &_Image);


	unsigned char *Data = nullptr;
private:   

	int Channels{ 4 };
	iVec2 Size{ 0 };
	size_t DataSize{ 0 };

	std::string Filepath{""};
};

