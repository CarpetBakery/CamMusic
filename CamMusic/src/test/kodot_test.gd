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

@export var handLeft: Control
@export var handRight: Control


func _ready() -> void:
	initialize()
	pass


func _process(delta: float) -> void:
	#update(delta)
	var joints := getSkeletonJoints(0)
	if joints.is_empty():
		return

	updateHands(joints)
	pass

func updateHands(joints: Dictionary[int, Vector2]):
	#print(joints)
	#if joints.has(SKELETON_POSITION_INDEX.HAND_LEFT):
		#handLeft.position = joints.get(SKELETON_POSITION_INDEX.HAND_LEFT)
	#if joints.has(SKELETON_POSITION_INDEX.HAND_RIGHT):
		#handRight.position = joints.get(SKELETON_POSITION_INDEX.HAND_RIGHT)
	pass
