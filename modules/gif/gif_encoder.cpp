/* GifEncoder.cpp */

#include "gif_encoder.h"
#include "gif.h"

void GifEncoderTest::begin(const String &p_path, int width, int height, int delay) {
    GifBegin(&gifwriter, p_path.utf8().get_data(), width, height, delay, true);

}
void GifEncoderTest::add_frame(const PoolVector<uint8_t> &data, int width, int height, int delay) {
    uint8_t* temp = (uint8_t*)malloc(width * height * 4 * sizeof(uint8_t));
    PoolVector<uint8_t>::Read r = data.read();
    const unsigned char *data_ptr = r.ptr();
    for(int i=0; i < data.size(); i++)
	    temp[i] = static_cast<uint8_t>(data_ptr[i]);
    GifWriteFrame(&gifwriter, temp, width, height, delay, 8, true);

}

void GifEncoderTest::end(){
    GifEnd(&gifwriter);
}

void GifEncoderTest::_bind_methods() {

    ClassDB::bind_method(D_METHOD("add_frame", "data", "width", "height", "delay"), &GifEncoderTest::add_frame);
    ClassDB::bind_method(D_METHOD("begin", "path","width", "height", "delay"), &GifEncoderTest::begin);
    ClassDB::bind_method(D_METHOD("end"), &GifEncoderTest::end);

}

GifEncoderTest::GifEncoderTest() {

}

