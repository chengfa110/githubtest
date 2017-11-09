#include <ros/ros.h>
#include <sstream>
#include <string>
#include <vector>
#include<sensor_msgs/LaserScan.h>
#include<nav_msgs/Odometry.h>
#include<wolf/alpha.h>
#include <iostream>
using namespace std;

	
class Distance{
  public :
    Distance(void){
    ros::param::get("~robot_this",robot_this);
    ros::param::get("/robot_goal",robot_goal);
    ros::param::get("robot_id",robot_id);
  }
  void start();
  private:
  ros::NodeHandle node;
  void distanceCB1(const nav_msgs::Odometry odometry);
  void distanceCB2(const nav_msgs::Odometry odometry);
  float coordinate[4];
  ros::Publisher alphaPub;
  ros::Subscriber odom0;
  ros::Subscriber odom1;
  std::string robot_goal;
  std::string robot_this;
  int robot_id;
};


void Distance::distanceCB1(const nav_msgs::Odometry odometry){
  coordinate[0]=odometry.pose.pose.position.x;
  coordinate[1]=odometry.pose.pose.position.y;
}

void Distance::distanceCB2(const nav_msgs::Odometry odometry){
  float oo,bb,distance;
  wolf::alpha alpha;
  coordinate[2]=odometry.pose.pose.position.x;
  coordinate[3]=odometry.pose.pose.position.y;
  oo=fabs(coordinate[0]-coordinate[2]);
  oo=pow(oo,2);
  bb=fabs(coordinate[1]-coordinate[3]);
  bb=pow(bb,2);
  distance=sqrt(oo+bb);
  //cout<<"distance0=="<<distance0<<endl;
  
  alpha.robot_id=robot_id;
  alpha.distance=distance;
  alpha.tan=fabs(coordinate[1]-odometry.pose.pose.position.y)/fabs(coordinate[0]-odometry.pose.pose.position.x);
  alpha.cos=fabs(coordinate[0]-odometry.pose.pose.position.x)/fabs(coordinate[1]-odometry.pose.pose.position.y);
  if(coordinate[0]>=odometry.pose.pose.position.x)
      alpha.x=1;
      else
      alpha.x=0;
     if(coordinate[1]>=odometry.pose.pose.position.y)
      alpha.y=1;
      else
      alpha.y=0;
     alphaPub.publish(alpha); 
}


void Distance::start(){
   ros::Rate rate(20);
   alphaPub=node.advertise<wolf::alpha>("/alpha",1);
   odom1=node.subscribe("/"+robot_this,1,&Distance::distanceCB2,this);
   odom0=node.subscribe
   ("/"+robot_goal,1,&Distance::distanceCB1,this);
   
   while(ros::ok()){
     ros::spin();
     rate.sleep();
   }
}

int main(int argc, char** argv){
  ros::init(argc, argv, "Distance");
  Distance distance;
  distance.start();
  
  return 0;
}
