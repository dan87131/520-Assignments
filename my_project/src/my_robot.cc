#include <iostream>
#include "my_robot.h"


using namespace enviro;
using namespace std;

void MyRobotController::init() {
    // Set the endpoint to (360, 280)
    goal_x = 364;
    goal_y = -265;
    // Teleport the robot to the origin and face it upwards
    teleport(0, 0, 0);
    velocity = 666;
    angular_velocity = 66;
    auto_velocity = 166;
    auto_angular_velocity = 166;
    state = "searching";
    searching_mode = 0;
    target_direction = 0;
    tolerance = 10;
}


void MyRobotController::update() {
    // Print the current position of the robot
    cout << "Current position: (" << position().x << ", " << position().y << ")" << endl;

    // Check if the robot is currently being guided to a target position
    if (self_guidance) {
        // Move the robot towards the target position
        move_toward(target_x, target_y, auto_velocity, auto_angular_velocity);

        // Check if the robot has reached the target position
        if (abs(position().x - target_x) < tolerance && abs(position().y - target_y) < tolerance) {
            // Stop the robot and reset the self_guidance flag
            self_guidance = false;
            apply_force(0, 0);
        }
    } else {
        // Fix the direction of the robot if it is not being guided to a target position
        if (abs(angle() - target_direction) > tolerance) {
            // Rotate the robot to face the target direction
            if (angle() - target_direction > tolerance) {
                apply_force(0, -angular_velocity);
            } else {
                apply_force(0, angular_velocity);
            }
        } else {
            // Teleport the robot to its current position and fix its angle
            teleport(position().x, position().y, target_direction);
            apply_force(0, 0);

            // Execute the appropriate behavior based on the current state
            if (state == "searching") {
                searching();
            } else if (state == "FollowingWall") {
                following_wall();
            } else if (state == "stop") {
               
            }
        }
    }
}




void MyRobotController::searching() {
    if (sensor_value(0) < 15) {
        // meet edge and turn to longer side
        double left_distance = sqrt(pow(sensor_value(1), 2) + pow(sensor_value(0), 2));
        double right_distance = sqrt(pow(sensor_value(2), 2) + pow(sensor_value(0), 2));

        if (left_distance < right_distance) {
            target_direction = turning("right");
            wall_direction = 1;  // 1 for left, -1 for right
        } else {
            target_direction = turning("left");
            wall_direction = -1;  // 1 for left, -1 for right
        }
        state = "FollowingWall";
   
    } else {
        apply_force(velocity, 0);
    }
}


void MyRobotController::following_wall() {
    // If the robot is too close to the wall, turn away from it
    if (sensor_value(0) < 10) {
        if (wall_direction == -1 && sensor_value(2) > 15) {
            // If the wall is to the left and there is space to turn left, turn left
            self_guidance = true;
            target_x = position().x + 20 * cos(target_direction - M_PI / 2);
            target_y = position().y + 20 * sin(target_direction - M_PI / 2);
            target_direction = target_direction - M_PI / 2;
        } else if (wall_direction == 1 && sensor_value(1) > 15) {
            // If the wall is to the right and there is space to turn right, turn right
            self_guidance = true;
            target_x = position().x + 20 * cos(target_direction + M_PI / 2);
            target_y = position().y + 20 * sin(target_direction + M_PI / 2);
            target_direction = target_direction + M_PI / 2;
        }
    } else {
        // If the robot is not too close to the wall, move forward
        apply_force(velocity, 0);
    }

    // If self_guidance is true, move towards the target_x and target_y coordinates
    if (self_guidance) {
        move_toward(target_x, target_y, auto_velocity, auto_angular_velocity);
        if (abs(position().x - target_x) < tolerance && abs(position().y - target_y) < tolerance) {
            // If the robot has reached the target coordinates, stop moving and update the target direction
            self_guidance = false;
            apply_force(0, 0);
            if (wall_direction == -1) {
                // If the wall is to the left, turn left
                target_direction = target_direction - M_PI / 2;
            } else {
                // If the wall is to the right, turn right
                target_direction = target_direction + M_PI / 2;
            }
        }
    } else {
        // If self_guidance is false, fix the direction towards the target direction
        if (abs(angle() - target_direction) <= tolerance) {
            // If the robot has reached the target direction, teleport to its current position and update the state
            teleport(position().x, position().y, target_direction);
            apply_force(0, 0);
            state = "searching";
        } else if (angle() - target_direction > tolerance) {
            // If the robot needs to turn left, apply a negative angular force
            apply_force(0, -angular_velocity);
        } else {
            // If the robot needs to turn right, apply a positive angular force
            apply_force(0, angular_velocity);
        }
    }
}


void MyRobotController::achieve() {
    //Switch to achieveGoal state
    state = "achieveGoal";

    // Calculate the angle to the goal point using atan2
    double dx = goal_x - position().x;
    double dy = goal_y - position().y;
    double target_angle = atan2(dy, dx);

    // Move towards the goal point
    while (abs(position().x - goal_x) > tolerance || abs(position().y - goal_y) > tolerance) {
        move_toward(goal_x, goal_y, velocity, angular_velocity);
        if (abs(angle() - target_angle) < tolerance && abs(position().x - goal_x) < tolerance && abs(position().y - goal_y) < tolerance) {
            // If the robot has reached the goal point, stop the robot
            apply_force(0, 0);
            cout << "Goal reached!" << endl;
            break;
        }
    }
}




double MyRobotController::turning(string s) {
    

    // define the direction of the turn based on input string
    int direction = (s == "left") ? -1 : 1;

    // calculate the current angle in degrees
    double curAngle = angle() * 180 / M_PI;

    // calculate the target angle in degrees
    double targetAngle = ((int)(curAngle + 90 * direction + 360) % 360) * M_PI / 180;
    return targetAngle;
}
