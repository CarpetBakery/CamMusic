class_name NoteMaster extends Node2D

@export var instrument: Instrument
@export var noteScaleType: Scale.Type
@export var noteVol := 1.0
var notePlayers: Array[NotePlayer]
var noteScale: Scale
var noteCount := 8

var targetPos: Vector2

@export_group("Nodes")
@export var instLabel: RichTextLabel
@export var objNotePlayer: PackedScene


func _ready() -> void:
	targetPos = position
	
	# Setup scale
	noteScale = Scale.new()
	noteScale.setType(noteScaleType, instrument)

	# Setup note players
	createNotePlayers(8)


func _process(delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept"):
		createNotePlayers(noteCount)
		#noteCount += 1
	
	position = lerp(position, targetPos, 0.4)


func createNotePlayers(amount: int):
	# Destroy old note players
	for np in notePlayers:
		np.queue_free()
	notePlayers.clear()
	
	#var angleInc: float = 180 / float(amount)
	var angleInc := 26.0
	var angleTotal := angleInc * amount
	var dist := 500.0 ## Distance from center
	var moveTime := 1.4
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

		# Setup sound
		inst.setScaleDegree(clamp(i, 0, Scale.SCALE_SIZE - 1), noteScale)

		var stream := noteScale.streams[inst.scaleDegree]
		inst.setStream(stream)
		inst.noteVol = noteVol



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
