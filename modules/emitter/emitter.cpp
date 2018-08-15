#include "emitter.h"
#include "math.h"
#include "engine.h"

void Emitter::do_stuff(){
}

void Emitter::_draw(){
    if (texture.is_null())
		return;

	//RID ci = get_canvas_item();
	if(!Engine::get_singleton()->is_editor_hint())
	{
	    for(int i=0; i < particles.size(); ++i)
	    {
	       // if(particles[i].time > particles[i].lifetime)
	       // {
	          //  particles[i].position = Vector2(0,0);
	        //    particles[i].velocity = Vector2(Math::random(-10,10), Math::random(-10,10));
	         //   particles[i].time = 0.0;
	        //}
	            
	        //particles[i].velocity += Vector2(0,1.0);
	        //particles[i].position += particles[i].velocity;
            draw_texture(texture, particles[i].position, Color(1,1,1,1));
            //particles[i].time += 0.1;
        }
    }

	//bool filter_clip;
	//Rect2 src_rect(0,0,64,64);
	//Rect2 dst_rect(0,0,64,64);
	//texture->draw_rect_region(ci, dst_rect, src_rect, Color(1, 1, 1), false, Ref<Texture>(), filter_clip);
}

void Emitter::set_texture(const Ref<Texture> &p_texture) {

	if (p_texture == texture)
		return;

	if (texture.is_valid())
		texture->remove_change_receptor(this);

	texture = p_texture;

	if (texture.is_valid())
		texture->add_change_receptor(this);

	update();
	//emit_signal("texture_changed");
	//item_rect_changed();
	//_change_notify("texture");
}

Ref<Texture> Emitter::get_texture() const {

	return texture;
}

void Emitter::_notification(int p_notification){
	switch (p_notification) {
		case NOTIFICATION_DRAW:
			_draw();
			break;
		case NOTIFICATION_PROCESS:
		    update();
		    break;
    }
}

void Emitter::_bind_methods(){
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &Emitter::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &Emitter::get_texture);
    ClassDB::bind_method(D_METHOD("do_stuff"), &Emitter::do_stuff);
    
    ADD_PROPERTYNZ(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
}

Emitter::Emitter(){
    for(int i=0; i < 2000; ++i)
    {
        Particle new_p;
        new_p.position = Vector2(0,0);
        new_p.velocity = Vector2(Math::random(-10,10), Math::random(-10,10));
        particles.push_back(new_p);
    }
}