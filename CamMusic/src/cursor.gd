class_name Cursor extends Node2D

@export var leftHand := false:
	set(_leftHand):
		leftHand = _leftHand
		
@export_group("Nodes")
@export var sprite: Sprite2D
@export var particles: CPUParticles2D
