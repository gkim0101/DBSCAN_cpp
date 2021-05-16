#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdint>

struct Point{

	double x,y,z;
	int32_t cluster;

	//initializer cluster -1 = unvisited
	Point():x(0.0),y(0.0),z(0.0),cluster(-1){}
	Point(double tx, double ty, double tz):x(tx),y(ty),z(tz),cluster(-1){}

};

void readFile(std::vector<Point> &points){
	std::string line, bit;
	std::ifstream file("benchmark_hepta.dat");
	Point tmp_p;
	if(file.is_open()){
		while(getline(file,line)){
			std::stringstream ss(line);
			getline(ss,bit,',');
			tmp_p.x=stod(bit);
			getline(ss,bit,',');
			tmp_p.y=stod(bit);
			getline(ss,bit,',');
			tmp_p.z=stod(bit);
			points.push_back(tmp_p);	
		}
	}
	//check file read
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		std::cout<<"x= "<<itr->x<<" y= "<<itr->y<<" z= "<<itr->z<<" cluster= "<<itr->cluster<<std::endl;
		
	}	

	file.close();
}

//L2 distance
double dist(Point p1, Point p2){
	return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z);
}

void clusterPoint(std::vector<Point> &points, double eps, int32_t minPts, int32_t &cluster, Point &p){
	int32_t counter=0;
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		if( dist(p,*itr) <= eps ){
			counter++;
			if( counter >= minPts  ){
				p.cluster = cluster;
				cluster++;
				break;
			}
		}

	}
}

void expandCluster(std::vector<Point> &points, double eps, int32_t cluster, Point &p){
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		if( itr->cluster == -1 ){
			if( dist(*itr,p) <= eps ){
				itr->cluster = cluster;
			}
		}
	}
}

void DBSCAN(std::vector<Point> &points,double eps, int32_t minPts){
	int32_t cluster = 0;
	//pivot point check
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		if(itr->cluster == -1){
			clusterPoint(points,eps,minPts,cluster,*itr);
			if(itr->cluster != -1){
				expandCluster(points,eps,cluster,*itr);
			}
		}
	}

	//print result
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		std::cout<<"x= "<<itr->x<<" y= "<<itr->y<<" z= "<<itr->z<<" cluster= "<<itr->cluster<<std::endl;
	}
	
}

void writeFile(std::vector<Point> &points){
	std::ofstream ofile("dbscan_result.dat");
	if(ofile.is_open()){
		for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
			ofile<<itr->x<<","<<itr->y<<","<<itr->z<<","<<itr->cluster<<"\n";
		}
	}
	ofile.close();
}

int main(){
	double eps = 0.8*0.8;
	int32_t minPts = 5;
	std::vector<Point> points;

	//timer
	auto strt=std::chrono::steady_clock::now();

	readFile(points);	
	DBSCAN(points, eps, minPts);
	writeFile(points);

	auto end=std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsed=end-strt;

	std::cout<<"elapsed time(s)"<<elapsed.count()<<std::endl;

	return 0;
}

