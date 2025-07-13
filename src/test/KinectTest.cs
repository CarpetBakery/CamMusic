using Godot;
using System;
using Microsoft.Kinect;
using System.Linq;
using System.Diagnostics;

/*
Try interfacing with the Kinect.

Microsoft's example code referenced
    MainWindow.xaml.cs
    
*/

public partial class KinectTest : Node3D
{
    public KinectSensor kinect { get; private set; }
    public string id { get; private set; }

    // -- Skeleton --
    private Skeleton[] skeletonData;

    public override void _Ready()
    {
        base._Ready();

        KinectSensorCollection sensors = KinectSensor.KinectSensors;
        if (sensors.Count <= 0)
        {
            GD.PrintErr("Error: No Kinect sensors found.");
            return;
        }

        // Get Kinect device
        kinect = sensors.First();
        Debug.Assert(kinect != null, "Error: Kinect is null.");
        id = kinect.DeviceConnectionId;

        GD.Print(kinect.Status);
        GD.Print(id);

        GetSkeleton();
    }

    public override void _Process(double delta)
    {
        base._Process(delta);
    }

    // public void KinectAllFramesReady(object sender, EventArgs e)
    public void GetSkeleton()
    {
        if (null == kinect || null == kinect.SkeletonStream)
        {
            return;
        }

        bool haveSkeletonData = false;
        using (SkeletonFrame skeletonFrame = kinect.SkeletonStream.OpenNextFrame(1000))
        {
            if (skeletonFrame != null)
            {
                if (skeletonData == null || skeletonData.Length != skeletonFrame.SkeletonArrayLength)
                {
                    skeletonData = new Skeleton[skeletonFrame.SkeletonArrayLength];
                }
                skeletonFrame.CopySkeletonDataTo(skeletonData);
                haveSkeletonData = true;
            }
        }

        if (!haveSkeletonData)
        {
            return;
        }

        // Do something with skeleton
        GD.Print(skeletonData);
    }
}
