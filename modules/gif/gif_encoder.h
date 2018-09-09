/* gifencoder.h */
#ifndef GIF_ENCODER_H
#define GIF_ENCODER_H

#include "reference.h"
#include "gif_struct.h"


class GifEncoderTest : public Reference {
    GDCLASS(GifEncoderTest, Reference)


    GifWriter gifwriter;

protected:
    static void _bind_methods();

public:
    void begin(const String &p_path, int width, int height, int delay);
    void add_frame(const PoolVector<uint8_t> &data, int width, int height, int delay);
    void end();
    //virtual ~GifEncoderTest() = 0;

    GifEncoderTest();
};

#endif
