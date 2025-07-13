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
    public KinectSensor sensor { get; private set; }
    public string id { get; private set; }

    // -- Skeleton --
    private Skeleton[] skeletonData;

    public override void _Ready()
    {
        base._Ready();

        return;

        KinectSensorCollection sensors = KinectSensor.KinectSensors;
        if (sensors.Count <= 0)
        {
            GD.PrintErr("Error: No Kinect sensors found.");
            return;
        }

        // Get Kinect device
        sensor = sensors.First();
        Debug.Assert(sensor != null && sensor.Status == KinectStatus.Connected, "Error: Kinect is invalid.");
        id = sensor.DeviceConnectionId;

        GD.Print(sensor.Status);
        GD.Print(id);

        // So we can receive skeleton frames
        sensor.SkeletonStream.Enable();
        // Add event handler to be called whenever there is new color frame data
        sensor.SkeletonFrameReady += SensorSkeletonFrameReady;
    }

    public override void _Process(double delta)
    {
        base._Process(delta);
    }

    private void SensorSkeletonFrameReady(object sender, SkeletonFrameReadyEventArgs e)
    {
        Skeleton[] skeletons = new Skeleton[0];
        using (SkeletonFrame skeletonFrame = e.OpenSkeletonFrame())
        {
            if (skeletonFrame != null)
            {
                skeletons = new Skeleton[skeletonFrame.SkeletonArrayLength];
                skeletonFrame.CopySkeletonDataTo(skeletons);
            }
        }
        // TODO: Do something with this
    }

    public void GetSkeleton()
    {
        if (null == sensor || null == sensor.SkeletonStream)
        {
            return;
        }

        bool haveSkeletonData = false;
        using (SkeletonFrame skeletonFrame = sensor.SkeletonStream.OpenNextFrame(1000))
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
