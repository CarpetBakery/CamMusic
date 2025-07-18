class_name Prim extends Node3D

@export var modelParent: Node3D
var animPlayer: AnimationPlayer


func _ready() -> void:
	for child in modelParent.get_children():
		if child is AnimationPlayer:
			animPlayer = child
			break
