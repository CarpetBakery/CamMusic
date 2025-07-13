using Godot;
using System;
using MeltySynth;

public partial class AudioGeneratorTest : Node
{	
	[ExportGroup("Nodes")]
	[Export] AudioStreamPlayer player;
	AudioStreamGeneratorPlayback playback;

	[ExportGroup("")]
	[Export] bool play = true;
	[Export] float pulseHz = 440.0f;
	float sampleHz = 22050.0f * 2.0f;
	float phase = 0.0f;

	public override void _Ready()
	{
		base._Ready();
		GD.Print("Testing AudioStreamGenerator playback...");

		player.Stream = new AudioStreamGenerator();
		player.Play(); // NEEDS to come before playback assignment
		playback = (AudioStreamGeneratorPlayback)player.GetStreamPlayback();

		if (playback == null)
		{
			GD.Print("That didn't work");
		}

		FillBuffer();
	}

	public override void _Process(double delta)
	{
		base._Process(delta);
		FillBuffer();
	}

	private void FillBuffer()
	{
		if (!play)
			return;

		float increment = pulseHz / sampleHz;
		int toFill = playback.GetFramesAvailable();

		while (toFill > 0)
		{
			playback.PushFrame(Vector2.One * (float)Mathf.Sin(phase * float.Tau));
			phase += increment;
			phase %= 1.0f;

			toFill--;
		}
	}
}
