class_name NoteMaster extends Node2D

@export var instrument: Instrument
@export var noteScale: Scale
@export var notePlayers: Array[NotePlayer]
@export var noteVol := 1.0

@export_group("Nodes")
@export var instLabel: RichTextLabel


func _ready() -> void:
	# Setup note scale
	setScaleType(noteScale.type)
	
	# Setup NotePlayers
	for np in notePlayers:
		# Clamp scale degree
		np.scaleDegree = clamp(np.scaleDegree, 0, Scale.SCALE_SIZE)
		
		# Set stream on player
		var stream := noteScale.streams[np.scaleDegree]
		np.setStream(stream)
		np.noteVol = noteVol



func setScaleType(newType: Scale.Type):
	noteScale.setType(newType, instrument)
	
	var instName := instrument.resource_path.split("inst")[-1]
	instLabel.text = "[tornado radius=3 freq=10]Instrument: [color=yellow]" + instName + "[/color]" + \
			"\nScale: [color=yellow]" + str(noteScale)
