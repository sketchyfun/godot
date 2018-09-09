/* register_types.cpp */

#include "register_types.h"
#include "class_db.h"
#include "gif_encoder.h"

void register_gif_types() {

        ClassDB::register_class<GifEncoderTest>();
}

void unregister_gif_types() {
   //nothing to do here
}
