#include <ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<wolf/alpha.h>
#include<wolf/alpha.h>
#include<nav_msgs/Odometry.h>
#include<wolf/attack.h>
#include <iostream>
using namespace std;

class avoid{
  public:
  void start();
  float speed=0.2;
  int energy=2000;
  private:
  ros::NodeHandle nh_;
  int robot_id;
  //ros::Subscriber cmdSub;
  ros::Publisher cmdPub;
  ros::Subscriber alphaSub;
  ros::Subscriber goalSub;
  ros::Subscriber odomSub1;
  ros::Subscriber odomSub2;
  //void cmdCB(const geometry_msgs1::Twist twist);
  void alphaCB(const wolf::alpha alpha);
  ros::Subscriber attackSub;
  void attackCB(const wolf::attack attack);
  bool alive=true;
  float distance[3][5]={{100.0,100.0,100.0,100.0,100.0},{100.0,100.0,100.0,100.0,100.0},{100.0,100.0,100.0,100.0,100.0}};
  int dee[2][5]={{0,0,0,0,0},{0,0,0,0,0}};
  int re_id;
  float dis1=100.0;
  float dis2=100.0;
  int robot_num=5;
  float three_dis[3][2]; //躲避两架飞机时获取位置,目标机位置0 + 追击飞机位置1,2
  float tan; //两架飞机连线中点与目标机的角度tan
  void goalCB(const nav_msgs::Odometry odometry);
  void odomCB1(const nav_msgs::Odometry odometry);
  void odomCB2(const nav_msgs::Odometry odometry);
  int direction_x,direction_y;//判断x,y轴的方向d
};

/*void avoid::decideCB(const wolf::decide decide){
  geometry_msgs::Twist twist;
  float xi;
  if(decide.distance>4.5)
  xi=2;
  else if(decide.distance<=4.5)
  xi=3.5;
   if(decide.tan<decide.cos){
  if(decide.x==1)
  twist.linear.x=speed*xi;
  else
  twist.linear.x=-speed*xi;
  if(decide.y==1)
  twist.linear.y=speed*decide.tan*xi;
  else
  twist.linear.y=-speed*decide.tan*xi;
  if(energy>0&&alive){
  cmdPub.publish(twist);
  cout<<"energy:"<<energy<<endl;
  energy--;
  }
  else{
  twist.linear.x=0;
  twist.linear.y=0;
  cmdPub.publish(twist);
  }
   }
   else if(decide.tan>=decide.cos){
  if(decide.x==1)
  twist.linear.x=speed*decide.cos*xi;
  else
  twist.linear.x=-speed*decide.cos*xi;
  if(decide.y==1)
  twist.linear.y=speed*xi;
  else
  twist.linear.y=-speed*xi;
  if(energy>0&&alive){
  cmdPub.publish(twist);
  cout<<"energy:"<<energy<<endl;
  energy--;
  }
  else{
  twist.linear.x=0;
  twist.linear.y=0;
  cmdPub.publish(twist);
  }
   }
}*/
void avoid::alphaCB(const wolf::alpha alpha){
  geometry_msgs::Twist twist;
  robot_id=alpha.robot_id-1;
  re_id=alpha.robot_id;
  distance[0][robot_id]=alpha.distance;
  distance[1][robot_id]=alpha.tan;
  distance[2][robot_id]=alpha.cos;
  dee[0][robot_id]=alpha.x;
  dee[1][robot_id]=alpha.y;
  int id=0;
  int id2=0;
  if(distance[0][0]<distance[0][1]){
    dis1=distance[0][0];
    dis2=distance[0][1];
    id=0;
    id2=1;
  }
  else{
    dis1=distance[0][1];
    dis2=distance[0][0];
    id=1;
    id2=0;
  }

  for(int i=2;i<robot_num;i++){
    if(distance[0][i]<=dis1){
      dis2=dis1;
      dis1=distance[0][i];
      id2=id;
      id=i;
      }
  }

    if(dis2>7){
      if(distance[1][id]<=distance[2][id]){
        if(dee[0][id]==0)
        twist.linear.x=-speed*3;
         else
         twist.linear.x=speed*3;
         if(dee[1][id]==0)
         twist.linear.y=-speed*distance[1][id]*3;
         else
         twist.linear.y=speed*distance[1][id]*3;
         cmdPub.publish(twist);
      }
      else if(distance[1][id]>distance[2][id]){
        if(dee[0][id]==0)
        twist.linear.x=-speed*distance[2][id]*3;
         else
         twist.linear.x=speed*distance[2][id]*3;
         if(dee[1][id]==0)
         twist.linear.y=-speed*3;
         else
         twist.linear.y=speed*3;
         cmdPub.publish(twist);
      }
    }
    else{
      cout<<"id:"<<id<<"   id2:"<<id2<<endl;
      stringstream ss1;
      ss1<<"/robot_"<<id+1<<"/base_pose_ground_truth";
      odomSub1 = nh_.subscribe(ss1.str(),1,&avoid::odomCB1,this);
      stringstream ss2;
      ss2<<"/robot_"<<id2+1<<"/base_pose_ground_truth";
      odomSub2 = nh_.subscribe(ss2.str(),1,&avoid::odomCB2,this);
      ros::spinOnce();
      /*if(direction_x == 1)
        twist.linear.x=speed*3;
      else
        twist.linear.x=-speed*3;
      if(direction_y == 1)
        twist.linear.y=speed * tan * 3;
      else
        twist.linear.y=-speed * tan * 3;*/
        twist.linear.x=-speed*3;
        twist.linear.y=-speed * tan * 3;
      cmdPub.publish(twist);
    }
}

void avoid::attackCB(const wolf::attack attack){
  if(attack.attack==2)
  alive=false;
}

void avoid::goalCB(const nav_msgs::Odometry odometry)
{
  three_dis[0][0] = odometry.pose.pose.position.x;
  three_dis[0][1] = odometry.pose.pose.position.y;
}

void avoid::odomCB1(const nav_msgs::Odometry odometry)
{
  three_dis[1][0] = odometry.pose.pose.position.x;
  three_dis[1][1] = odometry.pose.pose.position.y;
}

void avoid::odomCB2(const nav_msgs::Odometry odometry)
{
  three_dis[2][0] = odometry.pose.pose.position.x;
  three_dis[2][1] = odometry.pose.pose.position.y;
  float midpoint_x,midpoint_y;
  midpoint_x = (three_dis[2][0] + three_dis[1][0]) / 2;
  midpoint_y = (three_dis[2][1] + three_dis[1][1]) / 2;
  cout<<"midpoint_x:"<<midpoint_x<<"   midpoint_y:"<<midpoint_y<<endl;
  if(midpoint_x > three_dis[0][0])
    direction_x=1;
    else
      direction_x=0;
  if(midpoint_y > three_dis[0][1])
    direction_y=1;
    else
      direction_y=0;
  tan = fabs(midpoint_y - three_dis[0][1]) / fabs(midpoint_x - three_dis[0][0]);
}

void avoid::start(){
  ros::Rate rate(20);
  ros::param::get("robot_id",robot_id);
  stringstream ss;
  ss<<"/robot_"<<robot_id<<"/cmd_vel";
  stringstream ss3;
  ss3<<"/robot_"<<robot_id<<"/base_pose_ground_truth";
  cmdPub=nh_.advertise<geometry_msgs::Twist>(ss.str(),10);
  goalSub=nh_.subscribe(ss3.str(),1,&avoid::goalCB,this);
  alphaSub=nh_.subscribe("/alpha",1,&avoid::alphaCB,this);
  attackSub=nh_.subscribe("/attack",1,&avoid::attackCB,this);

  while(ros::ok()){
  ros::spin();
  rate.sleep();
  }
}

int main(int argc, char** argv){
  ros::init(argc,argv,"run");
  avoid avoid;
  avoid.start();
  return 0;
}