//insert the Headername here
#ifndef SPLINE_H
#define SPLINE_H

#include "core/engine.h"
#include "core/color.h"
#include "scene/3d/spatial.h"
#include "scene/3d/immediate_geometry.h"
#include "scene/3d/mesh_instance.h"
#include "scene/resources/primitive_meshes.h"

class Spline : public Spatial {
	GDCLASS(Spline, Spatial);

	bool closed_loop;
	Array points;
	float path_length;
	ImmediateGeometry *geo;
	Ref<SpatialMaterial> line_material;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
  Spline();
  ~Spline();

  int get_point_count();
  Vector3 get_spline_point(float t);
  float get_spline_tilt(float t);
  float get_spline_speed(float t);
  float get_differential(float t);
  void set_closed(bool c);
  bool get_closed();
private:
  void update_points();
  void draw_spline();
  void calc_spline_length();
  float calc_segment_length(int index);
  int wrap_index(int index);
  float interpolate(float t, float p0, float p1, float p2, float p3, float tension);
  Vector3 interpolate(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float tension);
};

class SplinePoint : public MeshInstance {
	GDCLASS(SplinePoint, MeshInstance);

	float tension;
	float speed;
	float tilt;

	Ref<SphereMesh> sphere_mesh;
	Ref<SpatialMaterial> sphere_material;


protected:
	static void _bind_methods();

	void _notification(int p_what);
public:
  SplinePoint();
  ~SplinePoint();

  void set_tension(float t);
  float get_tension();
  void set_speed(float s);
  float get_speed();
  void set_tilt(float t);
  float get_tilt();
};

#endif
