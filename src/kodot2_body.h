#ifndef KODOT2_BODY_H
#define KODOT2_BODY_H

#include <godot_cpp/classes/resource.hpp>


namespace godot
{
   class Kodot2Body : public Resource
	{
		GDCLASS(Kodot2Body, Resource)

	protected:
		static void _bind_methods();
	
	public:
		// Is this body currently being tracked?
		bool tracked = false;

		// Joint positions in 3D Space
		TypedArray<Vector3> joints;
		// Joint positions in 2D Space
		TypedArray<Vector3> joints2D;

		Kodot2Body();
		~Kodot2Body();

		bool isTracked();

		TypedArray<Vector3> getJointPositions3D();
		TypedArray<Vector2> getJointPositions2D();
	};
}


#endif // KODOT2_BODY_H