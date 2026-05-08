#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <tf/tf.h>
#include <vector>
#include <string>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

struct Waypoint {
    double x, y, yaw;
    std::string name;
};

move_base_msgs::MoveBaseGoal makeGoal(double x, double y, double yaw) {
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = x;
    goal.target_pose.pose.position.y = y;
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);
    return goal;
}

bool sendWaypoints(MoveBaseClient& client, const std::vector<Waypoint>& waypoints) {
    for (size_t i = 0; i < waypoints.size(); i++) {
        const auto& wp = waypoints[i];
        ROS_INFO("[%zu/%zu] go to: %s  (x=%.2f, y=%.2f)", i+1, waypoints.size(), wp.name.c_str(), wp.x, wp.y);

        client.sendGoal(makeGoal(wp.x, wp.y, wp.yaw));
        client.waitForResult();

        if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("daoda: %s", wp.name.c_str());
        } else {
            ROS_WARN("nodaoda: %s, state: %s", wp.name.c_str(), client.getState().toString().c_str());
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "send_goal_node");
    ros::NodeHandle nh("~");

    MoveBaseClient client("move_base", true);
    ROS_INFO("wait move_base serve...");
    if (!client.waitForServer(ros::Duration(10))) {
        ROS_ERROR("move_base no ok");
        return 1;
    }
    ROS_INFO("move_base ok");

   
    std::vector<Waypoint> waypoints = {
        { 2.0,  0.0,  0.0,    "目标点1 前方2m" },
        { 2.0,  1.5,  1.57,   "目标点2 右前方" },
        { 0.0,  1.5,  3.14,   "目标点3 右侧" },
        { 0.0,  0.0,  0.0,    "目标点4 回到起点" },
    };

    bool ok = sendWaypoints(client, waypoints);
    ROS_INFO(ok ? "complete" : "interrpute");
    return ok ? 0 : 1;
}
