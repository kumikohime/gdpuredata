extends GDPureDataHandle

func _ready():
	#open_patch("res://testfiles/sawtooth.pd")
	#close_patch("sawtooth")
	#open_patch("res://testfiles/A arpeggio.pd")
	#close_patch("A arpeggio")
	open_patch("res://testfiles/sin wave.pd")
	play()

func _input(event):
	if event.is_action_pressed("ui_accept"):
		send_bang("start")

	#for sin wave.pd
	if event.is_action_pressed("ui_cancel"):
		send_float("freq", 440.0)
