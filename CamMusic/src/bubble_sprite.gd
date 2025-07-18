class_name BubbleSprite extends Node2D

@export_group("Nodes")
@export var sprite: Sprite2D

var sinOffset = randf_range(0, PI)
var sinAmp := 4.0
var sinSpd := randf_range(0.002, 0.005)
var cosSpd := randf_range(0.002, 0.005)

func _process(delta: float) -> void:
	position.x = cos(sinOffset + Time.get_ticks_msec() * cosSpd) * sinAmp
	position.y = sin(sinOffset + Time.get_ticks_msec() * sinSpd) * sinAmp
	
