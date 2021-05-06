#include "register_types.h"
#include "core/class_db.h"
#include "spline.h"

void register_spline_types(){
	ClassDB::register_class<Spline>();
	ClassDB::register_class<SplinePoint>();
}

void unregister_spline_types() {
}
