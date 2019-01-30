#include "ros/ros.h"
#include "std_msgs/String.h"
#include <fstream>
#include <iostream>
#include <string>

class InjectorNode {
public:
  InjectorNode() {
    ros::NodeHandle node_handler;
    publisher_obj = node_handler.advertise<std_msgs::String>("inject", 1);
    file_name = "/home/indrajith/wscatkin/src/user_story_two/src/can_data.csv";
    GenerateAnomalyTimestamp();
  }
  void GenerateAnomalyTimestamp();

private:
  std::ifstream infile;
  std::string msg_id, vehicle_speed, engine_speed, driver_door, front_passenger,
      rear_left_door, rear_right_door, timestamp;
  std_msgs::String publisher_message;
  int line_count;
  std::string file_name;
  ros::Publisher publisher_obj;
};

void InjectorNode::GenerateAnomalyTimestamp() {
  line_count = 0;
  ros::Rate loop_rate(4);
  infile.open(file_name, std::ios::in);
  if (infile) {
    infile.ignore(500, '\n');
    while (ros::ok() || (!infile.eof())) {
      getline(infile, msg_id, ',');
      getline(infile, vehicle_speed, ',');
      getline(infile, engine_speed, ',');
      getline(infile, driver_door, ',');
      getline(infile, front_passenger, ',');
      getline(infile, rear_left_door, ',');
      getline(infile, rear_right_door, ',');
      infile >> timestamp;
      if ((rand() % ++line_count) == 0) {
        publisher_message.data = timestamp;
        publisher_obj.publish(publisher_message);
      }
      ros::spinOnce();
      loop_rate.sleep();
    }
  }
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "anomaly_injector");
  InjectorNode injector_node_obj;

  return 0;
}
