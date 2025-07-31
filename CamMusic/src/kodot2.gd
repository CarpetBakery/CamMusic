class_name KodotGD2 extends Kodot2

@export var handSens := 2.0
@export var sensorAngle := -4

var trackingSkeleton := false
var skeletonLostTimer: Timer
var skeletonTimeout := 2.0

@export_group("Nodes")
@export var noteMaster: NoteMaster
@export var handLeft: Hand
@export var handRight: Hand

@export var shoulderCenter: Node2D
@export var head: Node2D

@export var animSprite: AnimatedSprite2D


func _ready() -> void:
	init_kinect()
	# set_sensorAngle(sensorAngle)
	
	skeletonLostTimer = Timer.new()
	skeletonLostTimer.one_shot = true
	add_child(skeletonLostTimer)
	
	skeletonLostTimer.timeout.connect(lostSkeleton)
	
	animSprite.play("idle")


func _process(delta: float) -> void:
	update_kinect(delta)
	
	var joints := get_body_joint_positions_2d()
	if joints.is_empty():
		return
	
	skeletonLostTimer.start(skeletonTimeout)
	animSprite.play("look")
	
	if not trackingSkeleton:
		trackingSkeleton = true
		print_rich("[color=green]Skeleton was found.")
	
	updateHands(joints)


func updateHands(joints: Array[Vector2]):
	var hl: Vector2
	var hr: Vector2
	
	hl = joints.get(JointType.HandLeft)
	hr = joints.get(JointType.HandRight)

	# Transform hand positions
	hl -= shoulderCenter.position
	hr -= shoulderCenter.position
	
	hl *= handSens
	hr *= handSens
	
	hl += shoulderCenter.position
	hr += shoulderCenter.position
	
	handLeft.position = hl
	handRight.position = hr

	# Update shoulder/head positions
	shoulderCenter.position = joints.get(JointType.SpineShoulder)
	head.position = joints.get(JointType.Head)


func toVec2(vec: Vector3) -> Vector2:
	return (Vector2(vec.x + 1, -vec.y + 1) * 0.5) * screenSize
		


func lostSkeleton():
	trackingSkeleton = false
	print_rich("[color=red]Skeleton was lost...")
	animSprite.play("idle")
