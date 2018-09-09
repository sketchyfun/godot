#ifndef GIF_STRUCT_H
#define GIF_STRUCT_H

struct GifWriter
{
	FILE* f;
	uint8_t* oldImage;
	bool firstFrame;
};

#endif // GIF_STRUCT_H
