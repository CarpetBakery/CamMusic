class_name KodotGD extends Kodot

enum JointIndex
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

@export_group("Nodes")
@export var handLeft: Hand
@export var handRight: Hand


func _ready() -> void:
	initialize()
	set_sensorAngle(2)


func _process(delta: float) -> void:
	var joints: Dictionary[int, Vector3] = getSkeletonJoints3D()
	if joints.is_empty():
		return
	
	updateHands(joints)


func updateHands(joints: Dictionary[int, Vector3]):
	if joints.has(JointIndex.HAND_LEFT):
		handLeft.position = toVec2(joints.get(JointIndex.HAND_LEFT))
		#print("Update left hand " + str(handLeft.position))
	if joints.has(JointIndex.HAND_RIGHT):
		handRight.position = toVec2(joints.get(JointIndex.HAND_RIGHT))
		#print("Update right hand " + str(handRight.position))


func toVec2(vec: Vector3) -> Vector2:
	return (Vector2(vec.x + 1, -vec.y + 1) * 0.5) * screenSize
		
