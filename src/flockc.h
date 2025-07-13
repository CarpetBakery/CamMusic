#ifndef FLOCKC_H
#define FLOCKC_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

#include "boidc.h"

namespace godot
{
    class FlockC : public Node2D
    {
        GDCLASS(FlockC, Node2D)
        
    protected:
        static void _bind_methods();

    public:
        int cellSize = 5;
        TypedDictionary<Vector2i, Array> spatialGrid;

        int iterations = 2;
        int runsOffset = 0;
        double followWeight = 500.0;
        Vector2 targetPos;
        TypedArray<BoidC> boids;

        void detectNeighbors(double _visualRange);
        void cohesion(double _cohesionWeight);
        void separation(double _separationDistance, double _separationWeight);
        void alignment(double _alignmentWeight);
        void borders(double delta, Vector2 _envDims, double _bordersWeight);
        void moveTowardsTarget();

        void update(double delta, 
                double _visualRange, 
                double _cohesionWeight,
                double _separationDistance,
                double _separationWeight,
                double _alignmentWeight,
                double _bordersWeight,
                Vector2 _envDims
            );
        
            
        TypedArray<BoidC> getBoids();
        
        int getIterations();
        void setIterations(const int p_iterations);

        // -- Spatial grid --
        void rebuildSpatialGrid();
        Vector2i toCell(Vector2 pos);

        int getCellSize();
        void setCellSize(const int p_cellSize);

        Vector2 getTargetPos();
        void setTargetPos(const Vector2 p_targetPos);

        double getFollowWeight();
        void setFollowWeight(const double p_followWeight);

    };
}


#endif //FLOCKC_H