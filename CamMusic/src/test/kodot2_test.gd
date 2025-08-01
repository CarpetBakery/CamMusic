class_name Kodot2Test extends Kodot2

# For testing
var POS_INDEX_NAME := [
	"SpineBase",
	"SpineMid",
	"Neck",
	"Head",
	"ShoulderLeft",
	"ElbowLeft",
	"WristLeft",
	"HandLeft",
	"ShoulderRight",
	"ElbowRight",
	"WristRight",
	"HandRight",
	"HipLeft",
	"KneeLeft",
	"AnkleLeft",
	"FootLeft",
	"HipRight",
	"KneeRight",
	"AnkleRight",
	"FootRight",
	"SpineShoulder",
	"HandTipLeft",
	"ThumbLeft",
	"HandTipRight",
	"ThumbRight",
	"Count",
]

var HAND_STATE_NAME = [
	"Unknown",
	"NotTracked",
	"Open",
	"Closed",
	"Lasso",
]

@export_group("Nodes")
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
	kinect_init()
	skeletalInit()


func _process(delta: float) -> void:
	# NOTE: Kinect will update roughly 30 times a second(?)
	kinect_update()
	skeletalUpdate(delta)
	gestureUpdate(delta)


# -- Hand gesture demo --
func gestureUpdate(delta: float):
	# Method 1
	var leftHandState := get_body_left_hand_state()
	var rightHandState := get_body_right_hand_state()
	
	# Show hand state on hand labels
	var leftLabel := jointLabels[JointType.HandLeft]
	var rightLabel := jointLabels[JointType.HandRight]
	
	leftLabel.text = HAND_STATE_NAME[leftHandState]
	rightLabel.text = HAND_STATE_NAME[rightHandState]
	leftLabel.show()
	rightLabel.show()
	
	return
	
	# Method 2 (broken???)
	var body: Kodot2Body = get_first_tracked_body()
	if not body.isTracked():
		return
	
	leftHandState = body.get_left_hand_state()
	rightHandState = body.get_right_hand_state()
	
	# Show hand state on hand labels
	leftLabel.text = HAND_STATE_NAME[leftHandState]
	rightLabel.text = HAND_STATE_NAME[rightHandState]
	leftLabel.show()
	rightLabel.show()


# -- Skeletal tracking demo --
func skeletalInit():
	for i in range(JointType.Count):
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


func skeletalUpdate(delta: float):
	var jointPoints := get_body_joint_positions_2d()
	if jointPoints.is_empty():
		return
	
	if Input.is_action_just_pressed("ui_accept"):
		showJointNames = !showJointNames

	for i in range(JointType.Count):
		# NOTE: Coords are already corrected for now
		var square: ColorRect = jointSquares.get(i)
		square.position = jointPoints.get(i)
