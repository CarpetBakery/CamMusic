class_name Scale extends Resource

## Each scale has 8 notes and a type

enum Type
{
	MAJOR, 
	MINOR,
	BLUES,
}

enum Note
{
	C,
	C_SHARP,
	D,
	D_SHARP,
	E,
	F,
	F_SHARP,
	G,
	G_SHARP,
	A,
	A_SHARP,
	B,
	C_HIGH
}

const SCALE_SIZE := 8

@export var type: Type
var streams: Array[AudioStream]


func _init() -> void:
	pass


func setType(newType: Type, instrument: Instrument):
	type = newType
	streams.clear()
	
	# Setup new streams
	match type:
		Type.MAJOR:
			streams = [
				instrument.streams[Note.C],
				instrument.streams[Note.D],
				instrument.streams[Note.E],
				instrument.streams[Note.F],
				instrument.streams[Note.G],
				instrument.streams[Note.A],
				instrument.streams[Note.B],
				instrument.streams[Note.C_HIGH],
			]
		
		Type.MINOR:
			streams = [
				instrument.streams[Note.C],
				instrument.streams[Note.D],
				instrument.streams[Note.D_SHARP],
				instrument.streams[Note.F],
				instrument.streams[Note.G],
				instrument.streams[Note.G_SHARP],
				instrument.streams[Note.A_SHARP],
				instrument.streams[Note.C_HIGH],
			]
		
		Type.BLUES:
			streams = [
				instrument.streams[Note.C],
				instrument.streams[Note.D],
				instrument.streams[Note.D_SHARP],
				instrument.streams[Note.F],
				instrument.streams[Note.F_SHARP],
				instrument.streams[Note.G],
				instrument.streams[Note.A_SHARP],
				instrument.streams[Note.C_HIGH],
			]


func _to_string() -> String:
	match type:
		Type.MAJOR:
			return "Major"
		Type.MINOR:
			return "Minor"
		Type.BLUES:
			return "Blues"
	return ""
