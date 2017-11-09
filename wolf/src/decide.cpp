#include <ros/ros.h>
#include<wolf/alpha.h>
#include<wolf/decide.h>
#include <iostream>
using namespace std;

class decide{
  public :
  void start();
  private:
  ros::NodeHandle node;
  int robot_id;
  float dis1=100.0;
  float dis2=100.0;
  ros::Publisher decidePub;
  ros::Subscriber alphaSub;
  float distance[3][5]={{100.0,100.0,100.0,100.0,100.0},{100.0,100.0,100.0,100.0,100.0},{100.0,100.0,100.0,100.0,100.0}};
  int dee[2][5]={{0,0,0,0,0},{0,0,0,0,0}};
  int i=0;
  int cont=1;
  int robot_num=5;
  void alphaCB(const wolf::alpha alpha);
  int re_id;
  int energy=300;
};

void decide::alphaCB(const wolf::alpha alpha){
  wolf::decide decide;
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
  
  for(i=1;i<robot_num;i++){
     if(distance[0][i]<=dis1){
      dis2=dis1;
      id2=id;
      dis1=distance[0][i];
      id=i;
     }
    }
  if(re_id==alpha.robot_id){
    energy--;
    if(energy>0){
      cout<<"decide id:"<<id<<endl;
      decide.tan=distance[1][id];
      decide.distance=distance[0][id];
      decide.decide=id+1;
      decide.cos=distance[2][id];
      decide.x=dee[0][id];
      decide.y=dee[1][id];
      decidePub.publish(decide);
    }
    if(energy<=0){
      if(energy<=-300){
        energy=300;
      }
      cout<<id2<<"   energy:"<<energy<<endl;
      decide.tan=distance[1][id2];
      decide.distance=distance[0][id2];
      decide.decide=id2+1;
      decide.cos=distance[2][id2];
      decide.x=dee[0][id2];
      decide.y=dee[1][id2];
      decidePub.publish(decide);
    }
  }
  else if(re_id!=alpha.robot_id){
    re_id=alpha.robot_id;
    energy=300;
    decide.tan=distance[1][id];
    decide.distance=distance[0][id];
    decide.decide=id+1;
    decide.cos=distance[2][id];
    decide.x=dee[0][id];
    decide.y=dee[1][id];
    decidePub.publish(decide);
  }
}

void decide::start(){
  ros::Rate rate(10);
  decidePub=node.advertise<wolf::decide>("/decide",1);
  alphaSub=node.subscribe("/alpha",10,&decide::alphaCB,this);
  
  while(ros::ok()){
  ros::spin();
  rate.sleep();
  }
}

int main(int argc, char** argv){
  ros::init(argc, argv, "decide");
  decide a;
  a.start();
  
  return 0;
} 
