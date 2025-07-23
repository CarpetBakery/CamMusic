class_name Kodot2Test extends Kodot2

enum JointType
{
	SpineBase		= 0,
	SpineMid		= 1,
	Neck			= 2,
	Head			= 3,
	ShoulderLeft	= 4,
	ElbowLeft		= 5,
	WristLeft		= 6,
	HandLeft		= 7,
	ShoulderRight	= 8,
	ElbowRight		= 9,
	WristRight		= 10,
	HandRight		= 11,
	HipLeft			= 12,
	KneeLeft		= 13,
	AnkleLeft		= 14,
	FootLeft		= 15,
	HipRight		= 16,
	KneeRight		= 17,
	AnkleRight		= 18,
	FootRight		= 19,
	SpineShoulder	= 20,
	HandTipLeft		= 21,
	ThumbLeft		= 22,
	HandTipRight	= 23,
	ThumbRight		= 24,
	Count			= ( ThumbRight + 1 ) 
}

@export_group("Nodes")
@export var handLeft: Control
@export var handRight: Control


func _ready() -> void:
	initialize()
	

func _process(delta: float) -> void:
	update(delta)
	#print(getBodyCount())
	
	var joints: Dictionary[int, Vector3] = getBodyJointPositions3D(0)
	print(joints)
	if joints.is_empty():
		return
	
	# Update hands
	if joints.has(JointType.HandLeft):
		handLeft.position = joints.get(JointType.HandLeft)
	
	if joints.has(JointType.HandRight):
		handRight.position = joints.get(JointType.HandRight)
