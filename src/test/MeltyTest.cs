using Godot;
using MeltySynth;
using System;
using System.IO;

public partial class MeltyTest : Node
{
    [ExportGroup("Nodes")]
    [Export] AudioStreamPlayer player;
    AudioStreamGeneratorPlayback playback;

    Synthesizer synth;
    MidiFileSequencer sequencer;
    MidiFile midiFile;

    int sampleRate = 44100;
    bool isPlaying = false;

    string soundfontPath = "/assets/soundfont/TimGM6mb.sf2";
    string midiPath = "/assets/midi/ripplestar.mid";

    public override void _Ready()
    {
        base._Ready();

        // -- Setup audio playback --
        player.Stream = new AudioStreamGenerator();
        player.Play();
        playback = (AudioStreamGeneratorPlayback)player.GetStreamPlayback();


        // -- Setup MeltySynth --
        // Get correct file paths
        string curPath = Directory.GetCurrentDirectory().Replace("\\", "/");
        soundfontPath = curPath + soundfontPath;
        midiPath = curPath + midiPath;

        synth = new Synthesizer(soundfontPath, sampleRate);
        midiFile = new MidiFile(midiPath);
        sequencer = new MidiFileSequencer(synth);

        PlayMidi();
    }

    public override void _Process(double delta)
    {
        base._Process(delta);

        if (isPlaying)
        {
            int toFill = playback.GetFramesAvailable();
            if (toFill <= 0)
                return;
            
            Vector2[] buffer = new Vector2[toFill];
            float[] left = new float[toFill];
            float[] right = new float[toFill];
            sequencer.Render(left, right);

            // Transfer float buffers to a single buffer...
            for (int i = 0; i < toFill; i++)
            {
                buffer[i] = new Vector2(left[i], right[i]);
            }
            playback.PushBuffer(buffer);
        }
    }

    private void PlayMidi()
    {
        if (isPlaying)
            return;

        isPlaying = true;
        sequencer.Play(midiFile, true);
    }
}
