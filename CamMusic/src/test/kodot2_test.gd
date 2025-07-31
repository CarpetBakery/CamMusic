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
	initVisuals()


func _process(delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept"):
		showJointNames = !showJointNames
	
	kinect_update()
	
	
	var jointPoints := get_body_joint_positions_2d(-1)
	if jointPoints.is_empty():
		return
	
	# NOTE: This only happens 30ish times a second, which seems to be the polling rate of the Kinect2
	# Whenever we call "update", there's a very good chance there won't be any data
	updateJoints(jointPoints)


func initVisuals():
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


func updateJoints(jointPoints):
	for i in range(JointType.Count):
		# NOTE: Coords are already corrected for now
		var square: ColorRect = jointSquares.get(i)
		square.position = jointPoints.get(i)
