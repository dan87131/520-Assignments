#ifndef __MY_ROBOT_AGENT__H
#define __MY_ROBOT_AGENT__H

#include "enviro.h"

using namespace enviro;

class MyRobotController : public Process, public AgentInterface {

    public:
        MyRobotController() : Process(), AgentInterface() {}

        void init();
        void start() {}
        void update();
        void stop() {}

        void searching();
        void achieve();
        void following_wall();
        double turning(string s);
        double sensor_value(int index) {
            return AgentInterface::sensor_value(index);
        }

    private:
        std::string state;  // Current state of the robot
        bool achieveGoal = false;  // Flag indicating if the robot has achieved its goal
        bool turning_left = false;  // Flag indicating if the robot is currently turning left
        bool FollowingWall = false;  // Flag indicating if the robot is currently following a wall
        bool self_guidance = false;  // Flag indicating if the robot is currently moving towards a target position
        int wall_direction = 1;  // 1 for left, -1 for right (used when following a wall)
        int searching_mode = 0;  // 0 for go ahead, -1 for turn right, 1 for turn left (used when searching)
        double velocity = 0;  // Maximum velocity of the robot
        double angular_velocity = 0;  // Maximum angular velocity of the robot
        double auto_velocity = 0;  // Velocity used when moving towards a target position
        double auto_angular_velocity = 0;  // Angular velocity used when moving towards a target position
        double target_direction;  // Target direction (in radians) for the robot to face
        double tolerance;  // Tolerance (in pixels) for the robot to consider itself at a target position/direction
        double target_x = 0;  // Target x-coordinate for the robot to move towards (used when self_guidance is true)
        double target_y = 0;  // Target y-coordinate for the robot to move towards (used when self_guidance is true)
        double goal_x = 0;  // x-coordinate of the goal position (not currently used)
        double goal_y = 0;  // y-coordinate of the goal position (not currently used)
};

class MyRobot : public Agent {

    public:
        MyRobot(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }

    private:
        MyRobotController c;
};

DECLARE_INTERFACE(MyRobot)

#endif
