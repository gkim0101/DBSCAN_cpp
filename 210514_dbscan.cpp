#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <fstream>

#define NOISE -2;

struct Point{
	double x=0.0;
	double y=0.0;
	double z=0.0;
	int32_t cluster=-1; //not defined
};

double distance(Point p1, Point p2){
	return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z);
}

void readcsv(std::vector<Point> &points){
	std::string line,bit;
	std::ifstream file("benchmark_hepta.dat");
	Point tmp_p;

	if(file.is_open()){
		std::cout<<"file opened"<<std::endl;
		//if line ends : break
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

	file.close();
}

void printpoints(std::vector<Point> &points){
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		std::cout<<"x="<<itr->x<<" y="<<itr->y<<" z="<<itr->z<<" cluster="<<itr->cluster<<std::endl;
	}
}

void expandCluster(std::vector<Point>::iterator itr_begin, std::vector<Point>::iterator itr_end, double eps, int32_t cluster_num){
	for(std::vector<Point>::iterator itr=itr_begin;itr!=itr_end;itr++){
		//should check if it was visited
		if(itr->cluster == -1){
		if(distance(*itr_begin,*itr)<eps){
			itr->cluster=cluster_num;
			std::cout<<"clustered as "<<cluster_num<<std::endl;
		}
		}
	}
}

//call by starting points to reduce calculation cost
void clustering(std::vector<Point>::iterator itr_begin, std::vector<Point>::iterator itr_end,double eps, double minPts, int32_t &cluster_num){
	int32_t count=0;
	for(std::vector<Point>::iterator itr=itr_begin;itr!=itr_end;itr++){
		if( distance( *itr_begin , *itr ) < eps){
			count++;
		}

		//save calculation cost
		if(count>minPts){
			break;
		}
	}
	//set core point
	if( count > minPts ){
		itr_begin->cluster=cluster_num;
		expandCluster(itr_begin,itr_end,eps,cluster_num);
		cluster_num++;
	}else{
		itr_begin->cluster=NOISE;
	}
}

void dbscan(std::vector<Point> &points, double eps, double minPts){
	int32_t cluster_num=0;
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		if(itr->cluster == -1){ 
			//not visited
			
			//clustering function
			clustering(itr, points.end(),eps, minPts, cluster_num);
		}
	}	
}

int main(){
	double eps=3;
	double minPts = 5;
	std::vector<Point> points;
	readcsv( points );
	dbscan( points , eps , minPts );
	printpoints( points );
	
	return 0;
}

