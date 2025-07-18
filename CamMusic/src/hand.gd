class_name Hand extends Node2D

@export var isRightHand := true
@export var handColor := Color(1, 1, 1)

@export_group("Nodes")
@export var sprite: ColorRect


func _ready() -> void:
	sprite.color = handColor


func _process(delta: float) -> void:
	pass
