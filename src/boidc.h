#ifndef BOIDC_H
#define BOIDC_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/color_palette.hpp>

#include <godot_cpp/core/math.hpp>

namespace godot
{
    class BoidC : public Node2D
    {
        GDCLASS(BoidC, Node2D);

    private:

    protected:
        static void _bind_methods();

    public:
        BoidC();
        ~BoidC();

        void _ready() override;
        void _process(double delta) override;

        double maxVelocity =  280.0;
        double maxAcceleration =  1000.0;
        double rotationOffset = Math_PI / 2.0;
        double timeOutOfBorders = 0.0;

        Vector2 velocity = Vector2(0, 0);
        Vector2 acceleration = Vector2(0, 0);

        TypedArray<BoidC> neighbors;
        TypedArray<double> neighborDistances;


        // -- Get/set --
        double get_maxVelocity() const;
        void set_maxVelocity(const double p_maxVelocity);

        double get_maxAcceleration() const;
        void set_maxAcceleration(const double p_maxAcceleration);

    };
}

#endif // BOIDC_H