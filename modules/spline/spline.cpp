#include "spline.h"


float Spline::interpolate(float t, float p0, float p1, float p2, float p3, float tension) {

	float tSquared = t * t;
	float tCubed = tSquared * t;
	float pos = (-0.5 * tension * tCubed + tension * tSquared - 0.5f * tension * t) * p0;
	pos += (1.0f + 0.5f * tSquared * (tension - 6.0f) + 0.5f * tCubed * (4.0f - tension)) * p1;
	pos += (0.5f * tCubed * (tension - 4.0f) + 0.5f * tension * t - (tension - 3.0f) * tSquared) * p2;
	pos += (-0.5f * tension * tSquared + 0.5f * tension * tCubed) * p3;

	return pos;
}

Vector3 Spline::interpolate(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float tension) {

	float tSquared = t * t;
	float tCubed = tSquared * t;
	Vector3 pos = (-0.5 * tension * tCubed + tension * tSquared - 0.5f * tension * t) * p0;
	pos += (1.0f + 0.5f * tSquared * (tension - 6.0f) + 0.5f * tCubed * (4.0f - tension)) * p1;
	pos += (0.5f * tCubed * (tension - 4.0f) + 0.5f * tension * t - (tension - 3.0f) * tSquared) * p2;
	pos += (-0.5f * tension * tSquared + 0.5f * tension * tCubed) * p3;

	return pos;
}


//Bind all your methods used in this class
void Spline::_bind_methods(){
	ClassDB::bind_method(D_METHOD("get_point_count"), &Spline::get_point_count);
	ClassDB::bind_method(D_METHOD("get_spline_point", "t"), &Spline::get_spline_point);
	ClassDB::bind_method(D_METHOD("get_spline_tilt", "t"), &Spline::get_spline_tilt);
	ClassDB::bind_method(D_METHOD("get_spline_speed", "t"), &Spline::get_spline_speed);
	ClassDB::bind_method(D_METHOD("get_differential", "t"), &Spline::get_differential);
	ClassDB::bind_method(D_METHOD("set_closed", "closed"), &Spline::set_closed);
	ClassDB::bind_method(D_METHOD("get_closed"), &Spline::get_closed);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "closed_loop"), "set_closed", "get_closed");
}


void Spline::_notification(int p_what) {

	switch (p_what) {

		case NOTIFICATION_READY: {
			set_process(true);
			add_child(geo);
			update_points();
			draw_spline();

		} break;
		
		case NOTIFICATION_PROCESS: {

			update_points();
			calc_spline_length();
			draw_spline();

		} break;

		case NOTIFICATION_EXIT_TREE: {

		remove_child(geo);

		} break;
	}
}

void Spline::set_closed(bool c) {
	closed_loop = c;
}

bool Spline::get_closed() {
	return closed_loop;
}

void Spline::update_points() {

	points.clear();

	for (int i = 0; i < get_child_count(); i++) {

		SplinePoint *child = Object::cast_to<SplinePoint>(get_child(i));
		if (!child)
			continue;
		points.append(child);
	}
}

void Spline::draw_spline() {

	if (!Engine::get_singleton()->is_editor_hint())
		return;

	if (points.size() < 4)
		return;

	float step = 0.1f;

	geo->clear();
	geo->set_material_override(line_material);

	if (closed_loop)
	{

		geo->begin(Mesh::PRIMITIVE_LINES, Ref<Texture>());

		for (float i = 0.0f; i < points.size(); i += step)
		{
			Vector3 pos1 = get_spline_point(i);
			Vector3 pos2 = get_spline_point(i + 0.1f);
			Vector3 dir = (pos1 - pos2).normalized();
			geo->set_color(Color(1,1,1));
			geo->add_vertex(pos1);
			geo->add_vertex(pos2);
			geo->set_color(Color(0.279785, 1, 0.21875));
			geo->add_vertex(pos1);
			geo->add_vertex(pos1 + Vector3(0,1,0).rotated(dir,Math::deg2rad(get_spline_tilt(i))) * 1.0f);
		}

		geo->end();
	}
	else
	{
		float end = points.size() - 2.0f;

		geo->begin(Mesh::PRIMITIVE_LINES, Ref<Texture>());

		geo->set_color(Color(1,0,0));
		geo->add_vertex(get_spline_point(0.0f));
		geo->add_vertex(get_spline_point(1.0f));

		for (float i = 1.0f; i < points.size() - 2.0f; i += step)
		{
			Vector3 pos1 = get_spline_point(i);
			Vector3 pos2 = get_spline_point(i + 0.1f);
			Vector3 dir = (pos1 - pos2).normalized();
			geo->set_color(Color(1,1,1));
			geo->add_vertex(pos1);
			geo->add_vertex(pos2);
			geo->set_color(Color(0.279785, 1, 0.21875));
			geo->add_vertex(pos1);
			geo->add_vertex(pos1 + Vector3(0,1,0).rotated(dir,Math::deg2rad(get_spline_tilt(i))) * 1.0f);
		}

		geo->set_color(Color(1,0,0));
		geo->add_vertex(get_spline_point(end));
		geo->add_vertex(get_spline_point(end+1.0f));
		geo->end();

	}

}

int Spline::get_point_count() {
	return points.size();
}

Vector3 Spline::get_spline_point(float t) {

	int index = int(t);
	Vector3 p0 = Object::cast_to<SplinePoint>(points[wrap_index(index - 1)])->get_transform().origin;
	Vector3 p1 = Object::cast_to<SplinePoint>(points[wrap_index(index)])->get_transform().origin;
	Vector3 p2 = Object::cast_to<SplinePoint>(points[wrap_index(index + 1)])->get_transform().origin;
	Vector3 p3 = Object::cast_to<SplinePoint>(points[wrap_index(index + 2)])->get_transform().origin;

	float tension = Object::cast_to<SplinePoint>(points[wrap_index(index)])->get_tension();

	return interpolate(t-int(t), p0, p1, p2, p3, tension);
}

float Spline::get_spline_tilt(float t) {

	int index = int(t);
	//float p0 = Object::cast_to<SplinePoint>(points[wrap_index(index - 1)])->get_tilt();
	float p1 = Object::cast_to<SplinePoint>(points[wrap_index(index)])->get_tilt();
	float p2 = Object::cast_to<SplinePoint>(points[wrap_index(index + 1)])->get_tilt();
	//float p3 = Object::cast_to<SplinePoint>(points[wrap_index(index + 2)])->get_tilt();

	//float tension = Object::cast_to<SplinePoint>(points[wrap_index(index)])->get_tension();

	return Math::lerp(p1,p2,float(Math::ease(t-int(t), -2.2f)));
}

float Spline::get_spline_speed(float t) {

	int index = int(t);
	//float p0 = Object::cast_to<SplinePoint>(points[wrap_index(index - 1)])->get_speed();
	float p1 = Object::cast_to<SplinePoint>(points[wrap_index(index)])->get_speed();
	float p2 = Object::cast_to<SplinePoint>(points[wrap_index(index + 1)])->get_speed();
	//float p3 = Object::cast_to<SplinePoint>(points[wrap_index(index + 2)])->get_speed();


	//float tension = Object::cast_to<SplinePoint>(points[wrap_index(index)])->get_tension();

	return Math::lerp(p1,p2,float(Math::ease(t-int(t), -2.2f)));

}

float Spline::get_differential(float t) {
	int index = int(t);

	Vector3 p0 = Object::cast_to<SplinePoint>(points[wrap_index(index - 1)])->get_transform().origin;
	Vector3 p1 = Object::cast_to<SplinePoint>(points[wrap_index(index)])->get_transform().origin;
	Vector3 p2 = Object::cast_to<SplinePoint>(points[wrap_index(index + 1)])->get_transform().origin;
	Vector3 p3 = Object::cast_to<SplinePoint>(points[wrap_index(index + 2)])->get_transform().origin;

	t = t-int(t);

	Vector3 point = 0.5f * (p2 - p0 + 2.0f * t * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) + 3.0f * t * t * (3.0f * p1 + p3 - p0 - 3.0f * p2));

	return point.length();
}

void Spline::calc_spline_length() {

	path_length = 0.0f;

	for (int i = 0; i < points.size(); i++) {
		float seg_length = calc_segment_length(i);
		path_length += seg_length;
	}
}

float Spline::calc_segment_length(int index) {

	float length = 0.0f;
	float step = 0.01;

	Vector3 old_point = get_spline_point(float(index));
	Vector3 new_point;

	for (float i = 0.0f; i < 1.0f; i += step)
	{
		new_point = get_spline_point(float(index) + i);
		length += old_point.distance_to(new_point);
		old_point = new_point;
	}

	return length;
}


int Spline::wrap_index(int index) {
	return Math::wrapi(index, 0, points.size());
}


Spline::Spline() {

	closed_loop = true;
	path_length = 0.0f;

	geo = memnew(ImmediateGeometry);

	line_material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	line_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	line_material->set_line_width(3.0);
	line_material->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	line_material->set_flag(SpatialMaterial::FLAG_SRGB_VERTEX_COLOR, true);
	line_material->set_albedo(Color(1, 1, 1));

	//add_child(geo);

}

Spline::~Spline() {
	memdelete(geo);
}


void SplinePoint::set_tension(float t) {
	tension = t;
}

float SplinePoint::get_tension() {
	return tension;
}

void SplinePoint::set_speed(float s) {
	speed = s;
}

float SplinePoint::get_speed() {
	return speed;
}

void SplinePoint::set_tilt(float t) {
	tilt = t;
}

float SplinePoint::get_tilt() {
	return tilt;
}

void SplinePoint::_notification(int p_what) {

	switch (p_what) {

		case NOTIFICATION_READY: {

		if (!get_mesh().is_valid())
		{
			sphere_mesh.instance();
			set_mesh(sphere_mesh);
		}

		if (!get_material_override().is_valid())
		{
			sphere_material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
			sphere_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
			sphere_material->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
			sphere_material->set_flag(SpatialMaterial::FLAG_SRGB_VERTEX_COLOR, true);
			sphere_material->set_albedo(Color(1, 0.5, 0.5));

			set_material_override(sphere_material);
		}

		if (!Engine::get_singleton()->is_editor_hint())
			set_visible(false);

		} break;

		case NOTIFICATION_EXIT_TREE: {

		} break;
	}
}


SplinePoint::SplinePoint() {
	tension = 0.5f;
	speed = 1.0f;
	tilt = 0.0f;

	set_scale(Vector3(0.2,0.2,0.2));

}

SplinePoint::~SplinePoint() {
	if (sphere_mesh.is_valid())
		sphere_mesh.unref();
	if (sphere_material.is_valid())
		sphere_material.unref();
}


void SplinePoint::_bind_methods(){
	ClassDB::bind_method(D_METHOD("set_tension", "t"), &SplinePoint::set_tension);
	ClassDB::bind_method(D_METHOD("get_tension"), &SplinePoint::get_tension);
	ClassDB::bind_method(D_METHOD("set_speed", "s"), &SplinePoint::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &SplinePoint::get_speed);
	ClassDB::bind_method(D_METHOD("set_tilt", "t"), &SplinePoint::set_tilt);
	ClassDB::bind_method(D_METHOD("get_tilt"), &SplinePoint::get_tilt);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "tension", PROPERTY_HINT_RANGE, "0.001,1.0 ,0.001"), "set_tension", "get_tension");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "speed", PROPERTY_HINT_RANGE, "-1000.0,1000.0,0.001"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "tilt", PROPERTY_HINT_RANGE, "-3600.0,3600.0,0.001"), "set_tilt", "get_tilt");
}
