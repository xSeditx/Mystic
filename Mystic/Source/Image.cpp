#include"Image.h"
#include"OpenGL.h"
Bitmap::~Bitmap()
{
	delete[](Data);
	Data = NULL;
}


Bitmap::Bitmap(std::string file)
	:
	Filepath(file)
{
	Data = SOIL_load_image( Filepath.c_str(), &Size.x, &Size.y, &Channels, SOIL_LOAD_AUTO);

	if (Data == nullptr)
	{
		__debugbreak();
	}
	//==============================================================================================================================================================================
	// FLIP THE Image TO PLAY NICE WITH OPENGL
	//==============================================================================================================================================================================
	{
		size_t RowSize = Size.x * sizeof(unsigned char) * Channels;
		size_t ImageSize = RowSize * Size.y;

		unsigned char *TempImage = new unsigned char[ImageSize] {0};

		for_loop(I, Size.y)
		{
			memcpy(TempImage + ((size_t)I * RowSize), (Data + (ImageSize - (((size_t)I + 1) * RowSize))), RowSize);// Copy bottom row into top of Temp buffer
		}
        DataSize = ImageSize;
		memcpy(Data, TempImage, ImageSize);
		delete[](TempImage);
	}
	//==============================================================================================================================================================================
}

Bitmap::Bitmap(void *_data, Vec2 _dims, unsigned int _channels)
	:
	Size(_dims),
	Channels(_channels),
	Data((unsigned char*)_data)
{}

bool Bitmap::Reload()
{
	Data = SOIL_load_image(Filepath.c_str(), &Size.x, &Size.y, 0, SOIL_LOAD_AUTO);
	return true;
}

Bitmap& Bitmap::FlippedImage(Bitmap &_Image)
{
	Bitmap Pic;
    Pic.Data = new unsigned char[_Image.DataSize];
    Pic.Channels = _Image.Channels;
    Pic.DataSize = _Image.DataSize;
    Pic.Size = _Image.Size;

    //==============================================================================================================================================================================
    // FLIP THE Image TO PLAY NICE WITH OPENGL
    //==============================================================================================================================================================================
    {
        size_t RowSize = _Image.Size.x * sizeof(unsigned char) * _Image.Channels;
        size_t ImageSize = RowSize * _Image.Size.y;

        unsigned char *TempImage = new unsigned char[ImageSize] {0};

        for_loop(I, _Image.Size.y)
        {
            memcpy(TempImage + ((size_t)I * RowSize), (_Image.Data + (ImageSize - (((size_t)I + 1) * RowSize))), RowSize);// Copy bottom row into top of Temp buffer
        }

        memcpy(Pic.Data, TempImage, ImageSize);
        delete[](TempImage);
    }
    //==============================================================================================================================================================================

    return Pic;
}








//memcpy(TempImage + (ImageSize - RowSize), Data + RowPointer, RowSize);// Copy bottom row into top row
//memcpy(TempImage + RowPointer, Temp, RowSize);// Copy Temp into top row
//unsigned char *Temp = new unsigned char[Size.y *(sizeof(unsigned char) * 3)]; //Size.x * Size.y *(sizeof(unsigned char) * 4)];   //SOIL_load_Image(Filepath.c_str(), &Size.x, &Size.y, &Channels, SOIL_LOAD_RGB);

//copy temp buffer to lower row
//
//unsigned int textureid = SOIL_create_OGL_texture(
//	data,
//	width, height, channels,
//	SOIL_CREATE_NEW_ID,
//	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
//);
//
//
//
////	size_t RowSize = Size.x * sizeof(unsigned char);
//	size_t WorkingPtr = (Size.x * Size.y) - RowSize;
// 
//	unsigned char *TempBuffer = new unsigned char(RowSize);
//	unsigned char *UpperRow = new unsigned char(RowSize);
//
//	size_t ImageSize = (Size.x * Size.y * sizeof(unsigned char));
//
//	for_loop(Index, Size.y)
//	{
//	 //	memcpy(TempBuffer + (RowSize*Index), Data + WorkingPtr, RowSize);// Get the Bottom Row of Data
//	//	WorkingPtr -= RowSize;
//	}
//	//Data = TempBuffer;
//	//memcpy(TempBufer, UpperRow, RowSize);//Copy upper row to a temp buffer
////memcpy(Data + WorkingPtr, Data - RowSize, RowSize); //copy lower row to upper row

