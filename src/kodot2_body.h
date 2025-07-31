#ifndef KODOT2_BODY_H
#define KODOT2_BODY_H

#include <godot_cpp/classes/ref_counted.hpp>


namespace godot
{
   class Kodot2Body : public RefCounted
	{
		GDCLASS(Kodot2Body, RefCounted)

	protected:
		static void _bind_methods();
	
	public:
		bool tracked = false;

		TypedArray<Vector3> joints;
		TypedArray<Vector2> joints2D;

		// Hand gestures
		int leftHandState;
        int rightHandState;

		enum _HandState working;

		Kodot2Body();
		~Kodot2Body();

		// Is this body currently being tracked?
		bool isTracked();

		// Get joint positions as Vector3
		TypedArray<Vector3> getJointPositions3D();
		// Get joint positions as Vector2
		TypedArray<Vector2> getJointPositions2D();

		int getLeftHandState();
		int getRightHandState();
	};
}


#endif // KODOT2_BODY_H