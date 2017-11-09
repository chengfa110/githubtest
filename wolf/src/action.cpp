#include <ros/ros.h>
#include<wolf/alpha.h>
#include<wolf/decide.h>
#include<wolf/attack.h>
#include<geometry_msgs/Twist.h>
#include <iostream>
using namespace std;

class action{
  public:
  void start();
  float speed=0.3;
  private:
  ros::NodeHandle node;
  ros::Publisher cmdPub;
  ros::Publisher attackPub;
  void decideCB(const wolf::decide decide);
  ros::Subscriber decideSub;
  ros::Subscriber attackSub;
  void attackCB(const wolf::attack attack);
  int robot_id;
  float distance=100;
  int energy=200;
};

void action::decideCB(const wolf::decide decide){
 geometry_msgs::Twist twist;
 if(decide.tan<decide.cos){
  if(robot_id==decide.decide){
    distance=decide.distance;
    if(decide.x==1)
    twist.linear.x=speed*1.8;
    else
    twist.linear.x=-speed*1.8;
    if(decide.y==1)
    twist.linear.y=speed*decide.tan*1.8;
    else
    twist.linear.y=-speed*decide.tan*1.8;
    cmdPub.publish(twist);
  }
  else if(robot_id!=decide.decide){
    if(decide.x==1)
    twist.linear.x=speed;
    else
    twist.linear.x=-speed;
    if(decide.y==1)
    twist.linear.y=speed*decide.tan;
    else
    twist.linear.y=-speed*decide.tan;
    cmdPub.publish(twist);
    }
 }
 else if(decide.tan>=decide.cos){
  if(robot_id==decide.decide){
    distance=decide.distance;
    if(decide.x==1)
    twist.linear.x=speed*decide.cos*1.8;
    else
    twist.linear.x=-speed*decide.cos*1.8;
    if(decide.y==1)
    twist.linear.y=speed*1.8;
    else
    twist.linear.y=-speed*1.8;
    cmdPub.publish(twist);
  }
  else if(robot_id!=decide.decide){
    if(decide.x==1)
    twist.linear.x=speed*decide.cos;
    else
    twist.linear.x=-speed*decide.cos;
    if(decide.y==1)
    twist.linear.y=speed;
    else
    twist.linear.y=-speed;
    cmdPub.publish(twist);
    }
 }
}

void action::attackCB(const wolf::attack attack){
  wolf::attack attack1;
  if(attack.attack==1&&distance<=3){
    attack1.attack=2;
    cout<<distance<<endl;
    attackPub.publish(attack1);
   }
}

void action::start(){
  ros::Rate rate(10);
  ros::param::get("robot_id",robot_id);
  stringstream ss;
  ss<<"/robot_"<<robot_id<<"/cmd_vel";
  cmdPub=node.advertise<geometry_msgs::Twist>(ss.str(),10);
  attackPub=node.advertise<wolf::attack>("/attack",10);
  decideSub=node.subscribe("/decide",1,&action::decideCB,this);
  attackSub=node.subscribe("/attack",1,&action::attackCB,this);
  while(ros::ok()){
   ros::spin();
   rate.sleep();
  }

}


int main(int argc, char** argv){
  ros::init(argc, argv, "action");
  action action;
  action.start();
  
  return 0;
}