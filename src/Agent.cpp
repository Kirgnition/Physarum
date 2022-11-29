#include "ofApp.h"

agent::agent() :
    arm_angle(45 * (PI/180)),
    rotation_angle(45 * (PI/180)),
    sensor_offset(9),
    sensor_width(1),
    step_size(1),
    dep_t(5),
    p_random_direction(0.0),
    s_min(0)
{
    // starting values
    x = ofRandom(ofGetWidth());
    y = ofRandom(ofGetHeight());
    i = std::floor(x);
    j = std::floor(y);
    direction = ofRandom(2*PI);
}

int agent::sense(trailmap trail_map) {
    // compute sensors locations
    int width = ofGetWidth();
    int height = ofGetHeight();

    float FL_x = x + sensor_offset * std::cos(direction - rotation_angle);
    float FL_y = y + sensor_offset * std::sin(direction - rotation_angle);
    float F_x = x + sensor_offset * std::cos(direction);
    float F_y = y + sensor_offset * std::sin(direction);
    float FR_x = x + sensor_offset * std::cos(direction + rotation_angle);
    float FR_y = y + sensor_offset * std::sin(direction + rotation_angle);

    // handle out of bound cases by turning around
    if (
        (FL_x <= 0 || FL_x >= width) ||
        (FL_y <= 0 || FL_y >= height) ||
        (F_x <= 0 || FL_x >= width) ||
        (F_y <= 0 || FL_y >= height) ||
        (FR_x <= 0 || FL_x >= width) ||
        (FR_y <= 0 || FL_y >= height)
    ) {
        // "bounce"
        direction = 2*(PI - direction);
//        if (direction > PI) direction -= PI;
//        else direction += PI;
    }
    
    // sample trail values
    int FL = trail_map.get_trail(FL_x, FL_y);
    int F = trail_map.get_trail(F_x, F_y);
    int FR = trail_map.get_trail(FR_x, FR_y);

    // rotate based on trail values
    if ((F > FL) && (F > FR)) return 0;
    else if ((F < FL) && (F < FR)) {
        // rotate left or right randomly
        if (ofRandom(1) > 0.5) return 1;
        else return -1;
    }
    else if (FL < FR) return 1;
    else if (FR < FL) return -1;
    else return 0;
}

void agent::move() {
    float next_x, next_y;
    next_position(next_x, next_y);

    // check if out of bound, if so turn around
    if(next_x <= 0 || next_x >= ofGetWidth() || next_y <= 0 || next_y >= ofGetHeight()) {
        rotate((float)PI);
        return;
    }
    x = next_x;
    y = next_y;
    i = std::floor(x);
    j = std::floor(y);
}

void agent::rotate(const int where) {
    switch (where) {
        case 1:
            direction += rotation_angle;
            if(direction > 2*PI) direction -= 2*PI;
            break;
        case -1:
            direction -= rotation_angle;
            if (direction < 0) direction += 2*PI;
            break;
        default:
            break;
    }
}

void agent::rotate(const float angle) {
    direction += angle;
    // make sure direction is always within (0,2*PI)
    if(direction > 2*PI) direction -= 2*PI;
    else if (direction < 0) direction += 2*PI;
}

void agent::next_position(float &_x, float &_y) {
    // possibility of random new direction
    if(ofRandom(1) > 1-p_random_direction) new_direction();
    float next_x = x + step_size * std::cos(direction);
    float next_y = y + step_size * std::sin(direction);
    _x = next_x;
    _y = next_y;
}

void agent::deposit(trailmap trail_map) {
    trail_map.set(x, y, dep_t);
}

void agent::encircle(float r_min, float r_max, float angle) {
    // get center of the scene
    float width = (float)ofGetWidth();
    float height = (float)ofGetHeight();
    float center_x = width/2;
    float center_y = height/2;

    // get random position on circle of radius r_max and thickness r_max - r_min
    // the clever approach would be to find a function y(x) that given r_min , r_max, center_x, center_y and x
    // that, given a random x in a valid range, returns a random y such that (x,y) is in the circle

    // here i use the dumb approach of generating points at random and keeping only those which are in the correct spot
    // this should not impact performance too much since this function should be called only once per agent in ofApp::setup()
    while (true) {
        float rand_x = ofRandom(width);
        float rand_y = ofRandom(height);
        // check if point is valid
        if (
            std::pow(rand_x - center_x, 2) + std::pow(rand_y - center_y, 2) < std::pow(r_max, 2) &&
            std::pow(rand_x - center_x, 2) + std::pow(rand_y - center_y, 2) > std::pow(r_min, 2)
           ) {
            // set agent position
            x = rand_x;
            y = rand_y;
            // set agent angle according to position
            direction = std::atan((y - center_y)/(x - center_x)) + angle;
            return;
        }
    }
}

float agent::get_x() {
    return x;
}

float agent::get_y() {
    return y;
}

void agent::get_FL(const int which, float &_x, float&_y) {
    switch(which) {
    case -1:
        _x = x + sensor_offset * std::cos(direction - rotation_angle);
        _y = y + sensor_offset * std::sin(direction - rotation_angle);
        break;
    case 0:
        _x = x + sensor_offset * std::cos(direction);
        _y = y + sensor_offset * std::sin(direction);
        break;
    case 1:
        _x = x + sensor_offset * std::cos(direction + rotation_angle);
        _y = y + sensor_offset * std::sin(direction + rotation_angle);
        break;
    }
}

float agent::get_step_size() {
    return step_size;
}

float agent::get_direction() {
    return direction;
}

void agent::set_x(float _x) {
    x = _x;
}

void agent::set_y(float _y) {
    y = _y;
}

void agent::new_direction() {
    direction = ofRandom(2*PI);
}
