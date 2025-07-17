class_name KodotTest extends Kodot

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

# TEMP toString for JointIndex
var POS_INDEX_NAME = [
	"HIP_CENTER",
	"SPINE",
	"SHOULDER_CENTER",
	"HEAD",
	"SHOULDER_LEFT",
	"ELBOW_LEFT",
	"WRIST_LEFT",
	"HAND_LEFT",
	"SHOULDER_RIGHT",
	"ELBOW_RIGHT",
	"WRIST_RIGHT",
	"HAND_RIGHT",
	"HIP_LEFT",
	"KNEE_LEFT",
	"ANKLE_LEFT",
	"FOOT_LEFT",
	"HIP_RIGHT",
	"KNEE_RIGHT",
	"ANKLE_RIGHT",
	"FOOT_RIGHT",
	"COUNT",
]

## The maximum range of position coords the Kinect can output.
## (A guess for now. Can be changed later)
@export var posCoordRange = Vector2(320.0, 320.0)

@export_category("Cursor")
@export var cursorSens := 4.5
@export var handIndex := JointIndex.HAND_RIGHT
@export var shoulderIndex := JointIndex.SHOULDER_CENTER
@export var elbowIndex := JointIndex.ELBOW_RIGHT

@export_group("Nodes")
@export var cursor: Cursor
@export var handLeft: Control
@export var handRight: Control
@export var squaresParent: Control

var jointSquares: Dictionary[int, ColorRect]
var jointLabels: Dictionary[int, Label]
var showJointNames := true:
	set(_showJointNames):
		showJointNames = _showJointNames
		for key in jointLabels.keys():
			var label = jointLabels.get(key)
			label.visible = showJointNames


func _ready() -> void:
	initialize()
	
	# Set Kinect angle
	set_sensorAngle(-4)
	#set_sensorAngle(20)
	
	for i in range(JointIndex.COUNT):
		var s := 20
		var inst: ColorRect = ColorRect.new()
		inst.color = Color(randf_range(0, 1), randf_range(0, 1), randf_range(0, 1))
		inst.size = Vector2(s, s)
		squaresParent.add_child(inst)
		jointSquares.get_or_add(i, inst)
		
		# Create label
		var label := Label.new()
		label.text = POS_INDEX_NAME[i]
		label.add_theme_font_size_override("font_size", 20)
		inst.add_child(label)
		label.position.y -= 30
		jointLabels.get_or_add(i, label)


func _process(delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept"):
		showJointNames = !showJointNames
	
	#update(delta)
	var joints := getSkeletonJoints(0)
	if joints.is_empty():
		return
	
	updateJoints(joints)


func updateJoints(joints: Dictionary[int, Vector2]):
	updateCursorPos(joints)
	
	for i in range(JointIndex.COUNT):
		if not joints.has(i):
			continue
		
		# Correct coords
		var jointPos: Vector2 = joints.get(i)
		jointPos = jointPos / posCoordRange * screenSize
		#jointPos = jointPos * cursorSens
		
		var square: ColorRect = jointSquares.get(i)
		square.position = jointPos


func updateCursorPos_OLD(joints: Dictionary[int, Vector2]): 
	# TODO: Change for handedness

	if not joints.has(handIndex) or not joints.has(shoulderIndex) or not joints.has(elbowIndex):
		return

	var halfScreenSize := screenSize / 2
	var handPos: Vector2 = joints.get(handIndex)
	var shoulderPos: Vector2 = joints.get(shoulderIndex)
	var elbowPos: Vector2 = joints.get(elbowIndex)
	
	var armDist: float = handPos.distance_to(shoulderPos) 

	handPos -= elbowPos
	handPos = (handPos / armDist) * (screenSize / 2)
	handPos += screenSize/2
	
	cursor.position = handPos
	#print(armDist)
	
	#handPos -= shoulderPos
	#handPos = (handPos / armDist) * halfScreenSize
	#handPos += halfScreenSize
	#cursor.position = handPos


func updateCursorPos(joints: Dictionary[int, Vector2]): 
	# TODO: Change for handedness
	
	if not joints.has(handIndex) or not joints.has(shoulderIndex) or not joints.has(elbowIndex):
		return

	var halfScreenSize := screenSize / 2
	var handPos: Vector2 = joints.get(handIndex)
	var shoulderPos: Vector2 = joints.get(shoulderIndex)
	var elbowPos: Vector2 = joints.get(elbowIndex)

	## The new cursor pos
	var cPos := Vector2.ZERO

	# Transform coords to screen space
	#cPos = handPos * screenSize
	
	print(handPos)
	cursor.position = handPos * screenSize
