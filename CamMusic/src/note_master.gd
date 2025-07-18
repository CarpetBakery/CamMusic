class_name NoteMaster extends Node2D

@export var instrument: Instrument
@export var noteScale: Scale.Type
@export var noteVol := 1.0
var notePlayers: Array[NotePlayer]

@export_group("Nodes")
@export var instLabel: RichTextLabel
@export var objNotePlayer: PackedScene

var noteCount := 8

func _ready() -> void:
	createNotePlayers(8)


func _process(delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept"):
		createNotePlayers(noteCount)
		noteCount += 1


func createNotePlayers(amount: int):
	# Destroy old note players
	for np in notePlayers:
		np.queue_free()
	notePlayers.clear()
	
	#var angleInc = 180.0 / float(amount)
	var angleInc := 26.0
	var angleTotal := angleInc * amount
	var dist := 500.0 ## Distance from center
	var moveTime := 0.7
	var moveTimeInc = 0.08
	
	var spawnPos := Vector2.UP * dist
	spawnPos = spawnPos.rotated(-deg_to_rad(angleTotal - ((amount / 2 + 0.5) * angleInc)))
	
	for i in range(amount):
		var inst: NotePlayer = objNotePlayer.instantiate()
		inst.position = Vector2(0, 0)
		inst.setTargetPos(spawnPos, moveTime)
		
		add_child(inst)
		notePlayers.push_back(inst)
		
		spawnPos = spawnPos.rotated(deg_to_rad(angleInc))
		moveTime += moveTimeInc



#func _ready() -> void:
	## Setup note scale
	#setScaleType(noteScale.type)
	#
	## Setup NotePlayers
	#for np in notePlayers:
		## Clamp scale degree
		#np.scaleDegree = clamp(np.scaleDegree, 0, Scale.SCALE_SIZE)
		#
		## Set stream on player
		#var stream := noteScale.streams[np.scaleDegree]
		#np.setStream(stream)
		#np.noteVol = noteVol
#
#
#
#func setScaleType(newType: Scale.Type):
	#noteScale.setType(newType, instrument)
	#
	#var instName := instrument.resource_path.split("inst")[-1]
	#instLabel.text = "[tornado radius=3 freq=10]Instrument: [color=yellow]" + instName + "[/color]" + \
			#"\nScale: [color=yellow]" + str(noteScale)
