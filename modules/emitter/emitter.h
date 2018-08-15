#ifndef EMITTER_H
#define EMITTER_H

#include "scene/2d/node_2d.h"
#include "scene/resources/texture.h"

class Emitter : public Node2D {
    GDCLASS(Emitter, Node2D);
	struct Particle{
		Vector2 position;
		Vector2 velocity;
		float lifetime = 5.0;
		float time = 0.0;
	};

	Vector<Particle> particles;

	Ref<Texture> texture;
    
    protected:
        static void _bind_methods();
	void _notification(int p_notification);
	void _draw();
        
    public:
        void do_stuff();
	void set_texture(const Ref<Texture> &p_texture);
	Ref<Texture> get_texture() const;
        
        Emitter();
};

#endif
