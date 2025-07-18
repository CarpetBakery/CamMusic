class_name FadeEffect extends Sprite2D

@export var finalScale := 1.8
var fadeSpd := 0.25

func _ready() -> void:
	modulate.a = 1.0
	
	var tw = create_tween()
	scale = Vector2(1, 1)
	tw.set_trans(Tween.TRANS_CUBIC)
	tw.set_ease(Tween.EASE_OUT)
	tw.tween_property(self, "modulate:a", 0, fadeSpd)
	tw.parallel().tween_property(self, "scale", Vector2(finalScale, finalScale), fadeSpd)
	
	tw.finished.connect(queue_free)
