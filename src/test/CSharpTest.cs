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

    private void Test()
    {
        GD.Print("Working");
    }

    private void doSomething()
    {
        GD.Print("Something");
    }
}
