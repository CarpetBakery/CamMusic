extends Node

func _ready() -> void:
	Engine.max_fps = 60


func _process(delta: float) -> void:
	if Input.is_action_just_pressed("ui_cancel"):
		get_tree().quit()
	
	# Make sure we can still go fullscreen even if the player isn't present
	if Input.is_action_just_pressed("fullscreen"):
		toggleFullscreen()


func toggleFullscreen():
	var window := get_window()
	if window.mode == Window.MODE_WINDOWED:
		window.mode = Window.MODE_EXCLUSIVE_FULLSCREEN
	else:
		window.mode = Window.MODE_WINDOWED


## Cancel tween if it exists and is running
## Returns true if we cancelled, false otherwise
func cancelTween(tween: Tween) -> bool:
	if tween and tween.is_running():
		tween.kill()
		return true
	return false
	
