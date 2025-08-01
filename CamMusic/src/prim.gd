class_name Prim extends Node3D

@export var modelParent: Node3D
var animPlayer: AnimationPlayer


func _ready() -> void:
	for child in modelParent.get_children():
		if child is AnimationPlayer:
			animPlayer = child
			break
	
	assert(animPlayer, "Error: could not find Prim's animation player.")
	
	#animPlayer.play("rest")
	animPlayer.play("talk")
	animPlayer.animation_finished.connect(animFinished)


func animFinished(name: String):
	#animPlayer.play(name)
	pass
