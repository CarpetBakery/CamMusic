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
var notes: Array[Note]


func _init() -> void:
	pass


func setType(newType: Type, instrument: Instrument):
	type = newType
	notes.clear()
	streams.clear()
	
	# Setup new streams
	match type:
		Type.MAJOR:
			notes = [
				Note.C,
				Note.D,
				Note.E,
				Note.F,
				Note.G,
				Note.A,
				Note.B,
				Note.C_HIGH,
			]
		
		Type.MINOR:
			notes = [
				Note.C,
				Note.D,
				Note.D_SHARP,
				Note.F,
				Note.G,
				Note.G_SHARP,
				Note.A_SHARP,
				Note.C_HIGH,
			]
		
		Type.BLUES:
			notes = [
				Note.C,
				Note.D,
				Note.D_SHARP,
				Note.F,
				Note.F_SHARP,
				Note.G,
				Note.A_SHARP,
				Note.C_HIGH,
			]
	
	# Add streams to array
	for note in notes:
		streams.push_back(instrument.streams[note])


func _to_string() -> String:
	match type:
		Type.MAJOR:
			return "Major"
		Type.MINOR:
			return "Minor"
		Type.BLUES:
			return "Blues"
	return ""


static func noteToString(note: Note) -> String:
	var map := {
		Note.C: "C",
		Note.C_SHARP: "C#",
		Note.D: "D",
		Note.D_SHARP: "D#",
		Note.E: "E",
		Note.F: "F",
		Note.F_SHARP: "F#",
		Note.G: "G",
		Note.G_SHARP: "G#",
		Note.A: "A",
		Note.A_SHARP: "A#",
		Note.B: "B",
		Note.C_HIGH: "C2"
	}
	return map.get(note as int)
