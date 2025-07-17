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

@export var cursor: Cursor
@export var handLeft: Control
@export var handRight: Control
@export var squaresParent: Control
## The maximum range of position coords the Kinect can output
## (A guess for now. Can be changed later)
@export var posCoordRange = Vector2(320.0, 320.0)

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
	
	for i in range(SKELETON_POSITION_INDEX.COUNT):
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
	
	updateHands(joints)


func updateHands(joints: Dictionary[int, Vector2]):
	# print(joints)
	if joints.has(SKELETON_POSITION_INDEX.HAND_LEFT):
		#handLeft.position = joints.get(SKELETON_POSITION_INDEX.HAND_LEFT)
		
		# TODO: Do cursor movement by calculating distance from like shoulder or elbow
		#cursor.position = joints.get(SKELETON_POSITION_INDEX.HAND_LEFT) * 10 - Vector2(400, 400)
		pass
	
	if joints.has(SKELETON_POSITION_INDEX.HAND_RIGHT):
		#handRight.position = joints.get(SKELETON_POSITION_INDEX.HAND_RIGHT)
		cursor.position = joints.get(SKELETON_POSITION_INDEX.HAND_RIGHT) * 10 - Vector2(400, 400)
		pass
	
	for i in range(SKELETON_POSITION_INDEX.COUNT):
		if not joints.has(i):
			continue
		
		var jointPos: Vector2 = joints.get(i)
		
		# Correct coords
		#jointPos = jointPos / posCoordRange * screenSize
		jointPos = jointPos * 4.5
		
		if i == int(SKELETON_POSITION_INDEX.HAND_RIGHT):
			print(jointPos)
		var square: ColorRect = jointSquares.get(i)
		square.position = jointPos
		
