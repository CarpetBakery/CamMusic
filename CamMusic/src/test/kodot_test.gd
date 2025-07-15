class_name KodotTest extends Kodot

enum SKELETON_POSITION_INDEX
{
	HIP_CENTER = 0,
	SPINE,
	SHOULDER_CENTER,
	HEAD,
	SHOULDER_LEFT,
	ELBOW_LEFT,
	WRIST_LEFT,
	HAND_LEFT,
	SHOULDER_RIGHT,
	ELBOW_RIGHT,
	WRIST_RIGHT,
	HAND_RIGHT,
	HIP_LEFT,
	KNEE_LEFT,
	ANKLE_LEFT,
	FOOT_LEFT,
	HIP_RIGHT,
	KNEE_RIGHT,
	ANKLE_RIGHT,
	FOOT_RIGHT,
	COUNT
}


func _ready() -> void:
	initialize()


func _process(delta: float) -> void:
	#update(delta)
	var joints := getSkeletonJoints(0)
	
	if joints.keys().size() > 0:
		return
	
	print(joints)
