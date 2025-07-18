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

@export var handSens := 2.0
@export var sensorAngle := -4

var trackingSkeleton := false
var skeletonLostTimer: Timer
var skeletonTimeout := 4.0

@export_group("Nodes")
@export var noteMaster: NoteMaster
@export var handLeft: Hand
@export var handRight: Hand

@export var shoulderCenter: Node2D
@export var head: Node2D


func _ready() -> void:
	initialize()
	set_sensorAngle(sensorAngle)
	
	skeletonLostTimer = Timer.new()
	skeletonLostTimer.one_shot = true
	add_child(skeletonLostTimer)
	
	skeletonLostTimer.timeout.connect(lostSkeleton)


func _process(delta: float) -> void:
	var joints: Dictionary[int, Vector3] = getSkeletonJoints3D()
	if joints.is_empty():
		return
	
	skeletonLostTimer.start(skeletonTimeout)
	
	if not trackingSkeleton:
		trackingSkeleton = true
		print_rich("[color=green]Skeleton was found.")
	
	updateHands(joints)


func updateHands(joints: Dictionary[int, Vector3]):
	if joints.has(JointIndex.SHOULDER_CENTER):
		shoulderCenter.position = toVec2(joints.get(JointIndex.SHOULDER_CENTER))
	
	if joints.has(JointIndex.HEAD):
		head.position = toVec2(joints.get(JointIndex.HEAD))
	
	var hl: Vector2
	var hr: Vector2
	
	if joints.has(JointIndex.HAND_LEFT):
		hl = toVec2(joints.get(JointIndex.HAND_LEFT))
	
	if joints.has(JointIndex.HAND_RIGHT):
		hr = toVec2(joints.get(JointIndex.HAND_RIGHT))
	
	# Transform hand positions
	hl -= shoulderCenter.position
	hr -= shoulderCenter.position
	
	hl *= handSens
	hr *= handSens
	
	hl += shoulderCenter.position
	hr += shoulderCenter.position
	
	
	handLeft.position = hl
	handRight.position = hr


func toVec2(vec: Vector3) -> Vector2:
	return (Vector2(vec.x + 1, -vec.y + 1) * 0.5) * screenSize
		


func lostSkeleton():
	trackingSkeleton = false
	print_rich("[color=red]Skeleton was lost...")
