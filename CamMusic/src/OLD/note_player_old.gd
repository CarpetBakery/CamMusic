class_name NotePlayer_OLD extends Node2D

## Noteplayers have a scale and scale degree

@export_range(0, 7, 1) var scaleDegree: int = 0

@export_group("Nodes")
@export var colArea: Area2D
@export var spriteParent: Node2D
@export var sprite: Sprite2D
@export var noteSfx: AudioStreamPlayer
@export var bubbleParticles: CPUParticles2D
@export var fadeObj: PackedScene

var noteVol := 1.0
var bounceTw: Tween
var volTw: Tween

var moveTw: Tween
var moveTime := 0.3
var targetPos: Vector2


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
func hit():
	bubbleParticles.emitting = false
	bubbleParticles.emitting = true
	
	Global.cancelTween(volTw)
	noteSfx.volume_linear = noteVol
	noteSfx.play()
	
	Global.cancelTween(bounceTw)
	
	var s := 1.5
	spriteParent.scale = Vector2(s, s)
	
	bounceTw = create_tween()
	bounceTw.set_trans(Tween.TRANS_ELASTIC)
	bounceTw.set_ease(Tween.EASE_OUT)
	bounceTw.tween_property(spriteParent, "scale", Vector2(1, 1), 0.5)
	
	# Create fade effect
	var inst: FadeEffect = fadeObj.instantiate()
	inst.position = position
	inst.texture = sprite.texture
	get_parent().add_child(inst)


## Stop playing the note
func noteOff():
	Global.cancelTween(volTw)
	
	volTw = create_tween()
	volTw.tween_property(noteSfx, "volume_linear", 0.0, 0.4)


func setTargetPos(newTargetPos: Vector2, _moveTime := moveTime):
	moveTime = _moveTime
	targetPos = newTargetPos
	
	Global.cancelTween(moveTw)
	moveTw = create_tween()
	moveTw.set_trans(Tween.TRANS_ELASTIC)
	moveTw.set_ease(Tween.EASE_OUT)
	moveTw.tween_property(self, "position", targetPos, moveTime)
