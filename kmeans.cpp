#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
// viet ham tinh sse : Nam
// viet lai ham find dong 202 : Nghia
// xu ly ngoai le kmeans : Hoang Long
// bat dau lam bao cao : Phung Long
// doc ghi file, tim data : Viet
//19035898615013 tech
using namespace std;
class Point
{
private:
	int id_point, id_cluster;
	vector<double> values;
	int total_values;
	string name;

public:
	Point(int id_point, vector<double> &values, string name = "")
	{
		this->id_point = id_point;
		total_values = values.size();

		for(int i = 0; i < total_values; i++)
			this->values.push_back(values[i]);

		this->name = name;
		id_cluster = -1;
	}

	int getID()
	{
		return id_point;
	}

	void setCluster(int id_cluster)
	{
		this->id_cluster = id_cluster;
	}

	int getCluster()
	{
		return id_cluster;
	}

	double getValue(int index)
	{
		return values[index];
	}

	int getTotalValues()
	{
		return total_values;
	}

	void addValue(double value)
	{
		values.push_back(value);
	}

	string getName()
	{
		return name;
	}
};

class Cluster
{
private:
	int id_cluster;
	vector<double> central_values;
	vector<Point> points;

public:
	Cluster(int id_cluster, Point point)
	{
		this->id_cluster = id_cluster;

		int total_values = point.getTotalValues();

		for(int i = 0; i < total_values; i++)
			central_values.push_back(point.getValue(i));

		points.push_back(point);
	}

	void addPoint(Point point)
	{
		points.push_back(point);
	}

	bool removePoint(int id_point)
	{
		int total_points = points.size();

		for(int i = 0; i < total_points; i++)
		{
			if(points[i].getID() == id_point)
			{
				points.erase(points.begin() + i);
				return true;
			}
		}
		return false;
	}

	double getCentralValue(int index)
	{
		return central_values[index];
	}

	void setCentralValue(int index, double value)
	{
		central_values[index] = value;
	}

	Point getPoint(int index)
	{
		return points[index];
	}

	int getTotalPoints()
	{
		return points.size();
	}

	int getID()
	{
		return id_cluster;
	}
};
class KCluster{
	
private:
	int id_kCluster;
	int K;
	vector<Cluster> clusters;
	double sse;

public:
	KCluster(int K,int id_kCluster, vector<Cluster> &clusters, double sse)
	{
		this->K = K;
		this->id_kCluster = id_kCluster;
		for(int i = 0; i < K; i++)
			this->clusters.push_back(clusters[i]);
		this->sse = sse;
	}
	void addCluster(Cluster cluster)
	{
		clusters.push_back(cluster);
	}
	int getId(){
		return id_kCluster;
	}
	double getSSE(){
		return sse;
	}
	void setSSE(double sse){
		this->sse = sse;
	}
	Cluster getCluster(int index)
	{
		return clusters[index];
	}
	
};
class KMeans
{
private:
	int K; // number of clusters
	int total_values, total_points, max_iterations;
	double sse;
	vector<Cluster> clusters;

	// return ID of nearest center (uses euclidean distance)
	int getIDNearestCenter(Point point)
	{
		double sum = 0.0, min_dist;
		int id_cluster_center = 0;
		for(int i = 0; i < total_values; i++)
		{
			sum += pow(clusters[0].getCentralValue(i) -
					   point.getValue(i), 2.0);
		}

		min_dist = sqrt(sum);

		for(int i = 1; i < K; i++)
		{	
			double dist;
			sum = 0.0;
			
			for(int j = 0; j < total_values; j++)
			{
				sum += pow(clusters[i].getCentralValue(j) -
						   point.getValue(j), 2.0);
			}
			
			dist = sqrt(sum);
			
			if(dist < min_dist)
			{
				min_dist = dist;
				id_cluster_center = i;
			}
			
		}
		
		return id_cluster_center;
	}

public:
	KMeans(int K, int total_points, int total_values, int max_iterations)
	{
		this->K = K;
		this->total_points = total_points;
		this->total_values = total_values;
		this->max_iterations = max_iterations;
	}

	void run(vector<Point> & points)
	{
		if(K > total_points)
			return;

		vector<int> prohibited_indexes;

		// choose K distinct values for the centers of the clusters
		for(int i = 0; i < K; i++)
		{
			while(true)
			{
				int index_point = rand() % total_points;
				if(find(prohibited_indexes.begin(), prohibited_indexes.end(),
						index_point) == prohibited_indexes.end())
				{
					prohibited_indexes.push_back(index_point);
					points[index_point].setCluster(i);
					Cluster cluster(i, points[index_point]);
					clusters.push_back(cluster);
					break;
				}
			}
		}
		
		int iter = 0;
		vector<KCluster> kClusters;
		double minSSE = 1000000000000.0;
		double minSSE1 = 1000000000000.0;
		while(true)
		{
			KCluster kCluster(K,iter,clusters,0);
			bool done = true;
			// associates each point to the nearest center
			for(int i = 0; i < total_points; i++)
			{
				int id_old_cluster = points[i].getCluster();
				int id_nearest_center = getIDNearestCenter(points[i]);
				if(id_old_cluster != id_nearest_center)
				{
					if(id_old_cluster != -1)
						clusters[id_old_cluster].removePoint(points[i].getID());

					points[i].setCluster(id_nearest_center);
					clusters[id_nearest_center].addPoint(points[i]);
					done = false;
				}
			}
			// recalculating the center of each cluster
			for(int i = 0; i < K; i++)
			{
				for(int j = 0; j < total_values; j++)
				{
					int total_points_cluster = clusters[i].getTotalPoints();
					double sum = 0.0;
					if(total_points_cluster > 0)
					{
						for(int p = 0; p < total_points_cluster; p++)
							sum += clusters[i].getPoint(p).getValue(j);
						clusters[i].setCentralValue(j, sum / total_points_cluster);
					}
				}
			}
			/*// caculate sse
			cout<<"Caculate SSE of iterator "<<iter<<endl;
			double sum = 0.0;
			for(int i = 0; i < K; i++)
			{	
				int total_points_cluster =  clusters[i].getTotalPoints();
				for(int j = 0; j < total_points_cluster; j++)
				{
					for(int p = 0; p < total_values; p++){
						double temp = 0.0;
						temp = (clusters[i].getCentralValue(j) - points[j].getValue(j));
						sum += temp*temp;
					}	
				}
				
			}
			if(sum < minSSE){
				minSSE1 = minSSE;
				minSSE = sum;	
			}
			kCluster.setSSE(sum);
			cout<<"SSE of "<<kCluster.getId()<<" = "<<kCluster.getSSE()<<endl;
			cout<<"Print value of KCluster number "<< kCluster.getId()<<endl;
			for(int i = 0 ; i < K ; i ++ ){
				int total_points_cluster =  kCluster.getCluster(i).getTotalPoints();
				cout << "Cluster " << kCluster.getCluster(i).getID() + 1 << endl;
				for(int j = 0; j < total_points_cluster; j++)
				{
					cout << "Point " << kCluster.getCluster(i).getPoint(j).getID() + 1 << ": ";
					for(int p = 0; p < total_values; p++)
						cout << kCluster.getCluster(i).getPoint(j).getValue(p) << " ";
	
					string point_name = kCluster.getCluster(i).getPoint(j).getName();
	
					if(point_name != "")
						cout << "- " << point_name;
	
					cout << endl;
				}
	
				cout << "Cluster values: ";
	
				for(int j = 0; j < total_values; j++)
					cout << kCluster.getCluster(i).getCentralValue(j) << " ";
	
				cout << "\n\n";
			}*/
			if(done == true || iter >= max_iterations)
			{
				KCluster kCluster(K,iter,clusters,0);
				double sumFinal = 0.0;
				for(int i = 0; i < K; i++)
				{	
					int total_points_cluster =  clusters[i].getTotalPoints();
					cout<<"Total point of cluster "<<i<<" : "<<total_points_cluster<<endl;
					for(int j = 0; j < total_points_cluster; j++)
					{
						for(int p = 0; p < total_values; p++){
							double temp = 0.0;
							cout<<"a : "<<clusters[i].getCentralValue(p);
							cout<<" b : "<<clusters[i].getPoint(j).getValue(p)<<endl;
							temp = (clusters[i].getCentralValue(p) - clusters[i].getPoint(j).getValue(p));
							sumFinal += temp*temp;
						}	
					}
					
				}
				cout<<"Caculate SSE Final: "<<sumFinal<<endl;
				cout << "Break in iteration " << iter << "\n\n";
				break;
				
			}
			iter++;
		}
		// shows elements of clusters
		for(int i = 0; i < K; i++)
		{
			int total_points_cluster =  clusters[i].getTotalPoints();

			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			for(int j = 0; j < total_points_cluster; j++)
			{
				cout << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
				for(int p = 0; p < total_values; p++)
					cout << clusters[i].getPoint(j).getValue(p) << " ";

				string point_name = clusters[i].getPoint(j).getName();

				if(point_name != "")
					cout << "- " << point_name;

				cout << endl;
			}

			cout << "Cluster values: ";

			for(int j = 0; j < total_values; j++)
				cout << clusters[i].getCentralValue(j) << " ";

			cout << "\n\n";
		}
	}
};

int main(int argc, char *argv[])
{
	srand (time(0));

	int total_points, total_values, K, max_iterations, has_name;

	cin >> total_points >> total_values >> K >> max_iterations;

	vector<Point> points;
	string point_name;

	for(int i = 0; i < total_points; i++)
	{
		vector<double> values;

		for(int j = 0; j < total_values; j++)
		{
			double value;
			cin >> value;
			values.push_back(value);
		}

//		if(has_name)
//		{
//			cin >> point_name;
//			Point p(i, values, point_name);
//			points.push_back(p);
//		}
//		else
//		{
			Point p(i, values);
			points.push_back(p);
//		}
	}

	KMeans kmeans(K, total_points, total_values, max_iterations);
	kmeans.run(points);

	return 0;
}
