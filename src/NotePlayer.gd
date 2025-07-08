class_name NotePlayer extends Node2D

## Noteplayers have a scale and scale degree

@export_range(0, 7, 1) var scaleDegree: int = 0

@export_group("Nodes")
@export var colArea: Area2D
@export var spriteParent: Node2D
@export var noteSfx: AudioStreamPlayer

var noteVol := 1.0
var bounceTw: Tween
var volTw: Tween


func _ready() -> void:
	# -- Connections --
	colArea.mouse_entered.connect(hit)
	colArea.mouse_exited.connect(mouseExit)


func _process(delta: float) -> void:
	pass


func setStream(stream: AudioStream):
	noteSfx.stream = stream


func mouseEnter(): 
	hit()


func mouseExit():
	noteOff()


## Play sound, do bounce effect
## TODO
func hit():
	Global.cancelTween(volTw)
	noteSfx.volume_linear = noteVol
	noteSfx.play()
	
	Global.cancelTween(bounceTw)
	
	var s := 1.5
	scale = Vector2(s, s)
	
	bounceTw = create_tween()
	bounceTw.set_trans(Tween.TRANS_ELASTIC)
	bounceTw.set_ease(Tween.EASE_OUT)
	bounceTw.tween_property(self, "scale", Vector2(1, 1), 0.5)


## Stop playing the note
func noteOff():
	Global.cancelTween(volTw)
	
	volTw = create_tween()
	volTw.tween_property(noteSfx, "volume_linear", 0.0, 0.4)
