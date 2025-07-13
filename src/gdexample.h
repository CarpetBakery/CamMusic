#ifndef GDEXAMBLE_H
#define GDEXAMBLE_H

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot 
{
    class GDExample : public Sprite2D 
    {
        GDCLASS(GDExample, Sprite2D);

    private:
        double time_passed;
        double amplitude;
        double speed;

    protected:
        // Godot calls to find which functions/vars are exposed
        static void _bind_methods();

    public:
        GDExample();
        ~GDExample();

        // Just like in GDScript
        void _process(double delta) override;

        void test();

        void set_amplitude(const double p_amplitude);
        double get_amplitude() const;

        void set_speed(const double p_speed);
        double get_speed() const;
    };
}

#endif