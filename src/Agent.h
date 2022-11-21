#pragma once

#include "ofMain.h"
#include "Trail.h"
class agent{
public:
    int sense(trailmap trail_map);          // sense chemoattractant, input from trail_map and return -1,0,1 for rotate left,straight,right
    void rotate(const int where);           // rotate according to output from sense()
    void rotate(const float angle);         // rotate according to angle within range [0,2*PI)
    void next_position(float &x, float &y); // writes on variables x and y the next position of the agent
    void move();                            // move agent to next position
    void deposit(trailmap trail_map);       // deposit chemoattractant after moving

    // getters
    float get_x();
    float get_y();
    void get_FL(const int which, float &x, float&y);   // get sensor coordinates, which takes -1 for FL, 0 for F, 1 for FR
    float get_step_size();
    float get_direction();

    // setters
    void set_x(float _x);
    void set_y(float _y);
    void new_direction();

    agent();
private:
    // agent variables
    float x;                          // agent x coordinate
    float y;                          // agent y coordinate
    float i;                          // corresponding agent i index
    float j;                          // corresponding agent j index
    float direction;                  // current agent direction, from 0 to 2pi
    const float arm_angle;            // sensor arm angle
    const float rotation_angle;       // agent rotation angle
    const float sensor_offset;        // sensor arm length
    const float sensor_width;         // sensor size
    const float step_size;            // how far the agent moves per step
    const float dep_t;                // chemoattractant deposition per step
    const float p_random_direction;   // probability of random change in direction
    const float s_min;                // sensitivity to chemoattractant threshold
};
