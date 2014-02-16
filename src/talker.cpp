/***********************************************************************************************************************
FILENAME:   talker.cpp
AUTHORS:    Jesus Alcala

DESCRIPTION:
Creates a dummy occupancy map for testing purposes

PUBLISHES:  "GlobalMap" nav_msgs::OccupancyGrid
SUBSCRIBES: NA
SERVICES:   NA
***********************************************************************************************************************/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/GridCells.h"
#include "nav_msgs/OccupancyGrid.h"

nav_msgs::OccupancyGrid initializeOccupancyGrid(int length, double resolution);
nav_msgs::OccupancyGrid updateOccupancyGrid(nav_msgs::OccupancyGrid og, int x, int y);

double RESOLUTION;
int SIZE;
/***********************************************************************************************************************
nav_msgs::OccupancyGrid initializeOccupancyGrid(int length, double resolution)
Initialize an OccupancyGrid of size length*length
Returns the initialized OccupancyGrid
***********************************************************************************************************************/
nav_msgs::OccupancyGrid initializeOccupancyGrid(int length, double resolution) {
    RESOLUTION = resolution;
    SIZE  = length;
    nav_msgs::OccupancyGrid og;
    og.info.resolution = resolution;
    og.header.frame_id = "/world";
    og.info.origin.position.x = -SIZE/2;	//not sure why the negative or the divide by 2 but it works
    og.info.origin.position.y = -SIZE/2;	//not sure why the negative or the divide by 2 but it works
    og.header.stamp = ros::Time::now();
    og.info.width = SIZE;
    og.info.height = SIZE;
    og.data.resize(SIZE * SIZE);
    return og;
}

/***********************************************************************************************************************
nav_msgs::OccupancyGrid updateOccupancyGrid(nav_msgs::OccupancyGrid og, int x, int y,int t)
Updates a value in the OccupancyGrid.
int x: x coordinate
int y: y coordinate
int val: The value to insert in the OccupancyGrid
Returns the update OccupancyGrid 
***********************************************************************************************************************/
nav_msgs::OccupancyGrid updateOccupancyGrid(nav_msgs::OccupancyGrid og, int x, int y,int val) {
    og.data[((y*og.info.width)+x)] = val;
    return og;
}
/***********************************************************************************************************************
int main(int argc, char **argv)
program entry point
***********************************************************************************************************************/
int main(int argc, char **argv)
{
	ros::init(argc, argv, "talker");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<nav_msgs::OccupancyGrid>("GlobalMap", 1);
	ros::Rate loop_rate(10);

	int row,col,size=5;
	int map[5][5]= {	{ 0, 0, 0, 0, 0},			//Dummy Map
							{-1,-1,-1,-1, 0},
							{ 0, 0,-1,-1, 0},
							{ 0, 0,-1,-1, 0},
							{ 0, 0, 0, 0, 0}};
	nav_msgs::OccupancyGrid og= initializeOccupancyGrid(size,2);		//initialize a SIZExSIZE OccupancyGrid
	for(row=0;row<size;row++)
		for(col=0;col<size;col++)
			og=updateOccupancyGrid(og,col,row,map[row][col]);	//fill in OccupancyGrid with map

	while (ros::ok())
	{
		chatter_pub.publish(og);
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
