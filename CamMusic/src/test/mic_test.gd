class_name MicTest extends Node

@export var player: AudioStreamPlayer

func _ready() -> void:
	#print(AudioServer.get_input_device_list())
	AudioServer.input_device = AudioServer.get_input_device_list()[0]
	
	# Setup mic
	var micStream := AudioStreamMicrophone.new()
	
	player.stream = micStream
	player.play()
