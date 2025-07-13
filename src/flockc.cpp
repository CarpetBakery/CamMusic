#include "flockc.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;


void FlockC::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("update", "delta", "_visualRange",
            "_cohesionWeight",
            "_separationDistance",
            "_separationWeight",
            "_alignmentWeight",
            "_bordersWeight",
            "_envDims"
            ),
            &FlockC::update
        );

    ClassDB::bind_method(D_METHOD("detectNeighbors", "_visualRange"), &FlockC::detectNeighbors);
    ClassDB::bind_method(D_METHOD("cohesion", "_cohesionWeight"), &FlockC::cohesion);
    ClassDB::bind_method(D_METHOD("separation", "_separationDistance", "_separationWeight"), &FlockC::separation);
    ClassDB::bind_method(D_METHOD("alignment", "_alignmentWeight"), &FlockC::alignment);
    ClassDB::bind_method(D_METHOD("borders", "delta", "_envDims"), &FlockC::borders);

    // Get/set
    ClassDB::bind_method(D_METHOD("getBoids"), &FlockC::getBoids);

    ClassDB::bind_method(D_METHOD("getIterations"), &FlockC::getIterations);
    ClassDB::bind_method(D_METHOD("setIterations", "p_iterations"), &FlockC::setIterations);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "iterations"), "setIterations", "getIterations");

    ClassDB::bind_method(D_METHOD("getCellSize"), &FlockC::getCellSize);
    ClassDB::bind_method(D_METHOD("setCellSize", "p_CellSize"), &FlockC::setCellSize);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cellSize"), "setCellSize", "getCellSize");

    ClassDB::bind_method(D_METHOD("getTargetPos"), &FlockC::getTargetPos);
    ClassDB::bind_method(D_METHOD("setTargetPos", "p_TargetPos"), &FlockC::setTargetPos);

    ClassDB::bind_method(D_METHOD("getFollowWeight"), &FlockC::getFollowWeight);
    ClassDB::bind_method(D_METHOD("setFollowWeight", "p_FollowWeight"), &FlockC::setFollowWeight);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "followWeight"), "setFollowWeight", "getFollowWeight");
}

void FlockC::update(double delta, 
        double _visualRange, 
        double _cohesionWeight,
        double _separationDistance,
        double _separationWeight,
        double _alignmentWeight,
        double _bordersWeight,
        Vector2 _envDims
    )
{
    rebuildSpatialGrid();
    detectNeighbors(_visualRange);

    cohesion(_cohesionWeight);
    separation(_separationDistance, _separationWeight);
    alignment(_alignmentWeight);
    borders(delta, _envDims, _bordersWeight);

    moveTowardsTarget();
}

void FlockC::detectNeighbors(double _visualRange)
{
    // Clear all neighbor data
    size_t len = boids.size();

    // Figure out who each boid's neighbors are
    size_t runs = 0;
    for (int i = 0; i < len; i++)
    {
        BoidC* b1 = cast_to<BoidC>(boids.get(i));
        Vector2 pos = b1->get_position();
        Vector2i cell = toCell(pos);

        b1->neighbors.clear();
        b1->neighborDistances.clear();

        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                Vector2i nearCell = cell + Vector2i(x, y);
                if (!spatialGrid.has(nearCell))
                {
                    continue;
                }

                Array _arr = spatialGrid.get(nearCell, Array());
                size_t _len = _arr.size();
                for (int i = 0; i < _len; i++)
                {
                    BoidC* n = cast_to<BoidC>(_arr.get(i));
                    double dist = b1->get_position().distance_to(n->get_position());

                    if (dist <= _visualRange)
                    {
                        b1->neighbors.push_back(n);
                        b1->neighborDistances.push_back(dist);

                        n->neighbors.push_back(b1);
                        n->neighborDistances.push_back(dist);
                    }
                }
            }
        }
    }
}

void FlockC::cohesion(double _cohesionWeight)
{
    size_t len = boids.size();
    for (int i = 0; i < len; i++)
    {
        BoidC* b = cast_to<BoidC>(boids.get(i));
        if (b->neighbors.is_empty())
        {
            continue;
        }
        
        BoidC* closeBoid;
        TypedArray<BoidC> neighbors = b->neighbors;
        Vector2 averagePos = Vector2(0, 0);
        
        size_t neighborLen = b->neighbors.size();
        for (int j = 0; j < neighborLen; j++)
        {
            closeBoid = cast_to<BoidC>(b->neighbors.get(j));
            averagePos += closeBoid->get_position(); 
        }
        averagePos /= static_cast<double>(neighborLen);

        Vector2 dir = averagePos - b->get_position();
        b->acceleration += dir * _cohesionWeight;
    }
}

void FlockC::separation(double _separationDistance, double _separationWeight)
{
    size_t len = boids.size();
    for (int i = 0; i < len; i++)
    {
        BoidC* b = cast_to<BoidC>(boids.get(i));
        if (b->neighbors.is_empty())
        {
            continue;
        }
        
        size_t neighborLen = b->neighbors.size();
        for (int j = 0; j < neighborLen; j++)
        {
            BoidC* neighbor = cast_to<BoidC>(b->neighbors.get(j));

            double dist = b->neighborDistances.get(j);
            if (dist >= _separationDistance)
            {
                continue;
            }
            
            double distMult = 1.0 - (dist / _separationDistance);
            Vector2 direction = b->get_position() - neighbor->get_position();
            direction = direction.normalized();

            b->acceleration += direction * distMult * _separationWeight;
        }
    }
}

void FlockC::alignment(double _alignmentWeight)
{
    size_t len = boids.size();
    for (int i = 0; i < len; i++)
    {
        BoidC* b = cast_to<BoidC>(boids.get(i));
        TypedArray<BoidC> &neighbors = b->neighbors;

        if (neighbors.is_empty())
        {
            continue;
        }

        Vector2 averageVel = Vector2(0, 0);
        
        size_t neighborLen = neighbors.size();
        for (int j = 0; j < neighborLen; j++)
        {
            BoidC* n = cast_to<BoidC>(neighbors.get(j));
            averageVel += n->velocity;
        }
        averageVel /= static_cast<double>(neighborLen);

        b->acceleration += averageVel * _alignmentWeight;
    }
}

void FlockC::borders(double delta, Vector2 _envDims, double _bordersWeight)
{
    size_t len = boids.size();
    for (int i = 0; i < len; i++)
    {
        BoidC* b = cast_to<BoidC>(boids.get(i));
        Vector2 pos = b->get_position();

        if (pos.x < 0 || pos.x > _envDims.x || pos.y < 0 || pos.y > _envDims.y)
        {
			b->timeOutOfBorders += delta;
			Vector2 midPoint = _envDims / 2;
			Vector2 dir = (midPoint - b->get_position()).normalized();
			b->acceleration += dir * b->timeOutOfBorders * _bordersWeight;
        }
		else
        {
			b->timeOutOfBorders = 0;
        }
    }
}


TypedArray<BoidC> FlockC::getBoids()
{
    return boids;
}

int FlockC::getIterations()
{
    return iterations;
}
void FlockC::setIterations(const int p_iterations)
{
    iterations = p_iterations;
}


void FlockC::rebuildSpatialGrid()
{
    spatialGrid.clear();

    size_t len = boids.size();
    for (int i = 0; i < len; i++)
    {
        BoidC* b = cast_to<BoidC>(boids.get(i));
        Vector2i cell = toCell(b->get_position());
        
        if (!spatialGrid.has(cell))
        {
            spatialGrid.get_or_add(cell, Array());
        }
        Array arr = spatialGrid.get(cell, Array());
        arr.append(b);
    }
}

void FlockC::moveTowardsTarget()
{
    size_t len = boids.size();
    for (int i = 0; i < len; i++)
    {
        BoidC* b = cast_to<BoidC>(boids.get(i));

        // Move towards target pos
        Vector2 targetDir = Vector2(b->get_position() - targetPos).normalized();
        b->acceleration -= targetDir * followWeight;
    }
}

Vector2i FlockC::toCell(Vector2 pos)
{
    return Vector2i(floor(pos.x / cellSize), floor(pos.y / cellSize));
}


int FlockC::getCellSize()
{
    return cellSize;
}

void FlockC::setCellSize(const int p_cellSize)
{
    cellSize = p_cellSize;
}

Vector2 FlockC::getTargetPos()
{
    return targetPos;
}

void FlockC::setTargetPos(const Vector2 p_targetPos)
{
    targetPos = p_targetPos;
}

double FlockC::getFollowWeight()
{
    return followWeight;
}
void FlockC::setFollowWeight(const double p_followWeight)
{
    followWeight = p_followWeight;
}
