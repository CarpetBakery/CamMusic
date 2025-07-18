class_name BubbleParticles extends CPUParticles2D

func _ready() -> void:
	emitting = true
	finished.connect(queue_free)
