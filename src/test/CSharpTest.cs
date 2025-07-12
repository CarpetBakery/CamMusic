using Godot;
using System;
using System.Diagnostics;

public partial class CSharpTest : Node
{
    public override void _Process(double delta)
    {
        base._Process(delta);

        if (Input.IsActionJustPressed("ui_accept"))
        {
            Test();
        }
    }

	public void Test()
	{
        GD.Print("Working");
	}
}
