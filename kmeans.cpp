#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace std;
double minSSE = 1000000000000.0;
int idMinSSE = 0;
class Point
{
private:
	int id_point, id_cluster;
	vector<double> values;
	int total_values;

public:
	Point(int id_point, vector<double> &values)
	{
		this->id_point = id_point;
		total_values = values.size();
		for(int i = 0; i < total_values; i++)
			this->values.push_back(values[i]);
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
vector<KCluster> kClusters;
class KMeans
{
private:
	int K; // so luong cum
	int total_values, total_points, max_iterations;
	vector<Cluster> clusters;
	// Tra ve ID cua cum gan voi diem dang xet nhat
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
	
	bool findRandCenter(vector<int> a , int c){
	 	bool isAppeared = false;
		if(a.size() == 0){
			a.push_back(c);
		}
		else{
			for (int i = 0; i< a.size() ; i++){
	 			if(a[i] == c){
	 				isAppeared = true;
	 				return true;
				}
				else{
					isAppeared = false;
				}
			}
			return false;
		}

	}
	void run(vector<Point> & points)
	{
	
			if(K > total_points){
			cout<< " enrorr K > so luong dataset input";
			return;
		}
			
		// mo file output
		ofstream output;
        output.open("output.txt", ios::out | ios::trunc); //ios::trunc neu da co file output tu dong ghi de
		vector<int> prohibited_indexes;

		// Chon ra K diem ngau nhien lam tam
		for(int i = 0; i < K; i++)
		{
			while(true)
			{
				int index_point = rand() % total_points;

				if(!findRandCenter(prohibited_indexes, index_point)) // kiem tra xem diem do da duoc xet lam tam chua
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
		while(true)
		{
			KCluster kCluster(K,iter,clusters,0);
			bool done = true;

			// tim cum gan nhat cho diem
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

			// tinh lai tam cho moi cum
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
		
			if(done == true || iter >= max_iterations)
			{
				//xu ly doc file
            	output << "Break in iteration " << iter << endl;
				cout << "Break in iteration " << iter << "\n\n";
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
							temp = (clusters[i].getCentralValue(p) - clusters[i].getPoint(j).getValue(p));
							sumFinal += temp*temp;
						}	
					}
					
				}
				kCluster.setSSE(sumFinal);
				kClusters.push_back(kCluster);
				if(sumFinal < minSSE ){
					minSSE = sumFinal;
					idMinSSE = kClusters.size() - 1;
				}
		
				cout<<"_____Check vector size: "<<kClusters.size()<<endl;
				cout<<"Caculate SSE Final: "<<sumFinal<<endl;
				break;
			}

			iter++;
		}

		// hien thi ra cac diem trong moi cum
		for(int i = 0; i < K; i++)
		{
			int total_points_cluster =  clusters[i].getTotalPoints();

			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			output << "Cluster " << clusters[i].getID() + 1 << endl; //xu ly doc file
			for(int j = 0; j < total_points_cluster; j++)
			{
				cout << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
				output << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
				for(int p = 0; p < total_values; p++){
					cout << clusters[i].getPoint(j).getValue(p) << " ";
					output << clusters[i].getPoint(j).getValue(p) << " "; //xu ly doc file
				}
				cout << endl;
				output << endl;
			}

			cout << "Cluster values: ";
			output << "Cluster values: ";
			for(int j = 0; j < total_values; j++){
					cout << clusters[i].getCentralValue(j) << " ";
					output << clusters[i].getCentralValue(j) << " ";//xu ly doc file
			}
				output<< endl << endl;
				cout << "\n\n";
		}
        output.close(); // dong file output	
	}
		
};

int main(int argc, char *argv[])
{
	srand (time(0));
	int  K, max_iterations,total_values;
	int total_points = 0;
	cin >> K >> max_iterations;
	// lay du lieu tu trong file
    int pointId = 1;
	vector<Point> points;
	string line;
	string point_name;
	//	Mo file input.txt
    ifstream infile("sobar-72.csv");

    if (!infile.is_open())
    {
        cout << "Error: Failed to open file." << endl;
        return 1;
    }
    
 	while (getline(infile, line)) //doc den het file
    {
    	// xu ly lay gia tri tung dong trong file txt
		vector<double> values;
        string tmp = "";
        for (int i = 0; i < (int)line.length(); i++)
        {
            if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.' || line[i] == '+' || line[i] == '-' || line[i] == 'e')
            {
                tmp += line[i];
            }
            else if (tmp.length() > 0)
            {
            	double value= atof(tmp.c_str());
				values.push_back(value);
                tmp = "";
            }
        }
        if (tmp.length() > 0)
        {
        	double value= atof(tmp.c_str());
            values.push_back(value);
            tmp = "";
        } ;
			total_values = values.size(); // lay so thuoc tinh co trong 1 dataset
			Point p(pointId, values);
			points.push_back(p);
			pointId++;
			total_points+=1; // dem so dataset trong file truyen vao
			
    }
	cout<< total_points<< endl;
	for(int i = 0; i< 5 ; i++){
		cout<<"______BEGIN_____"<<endl;
		cout<<i<<"time"<<endl;
		KMeans kmeans(K, total_points, total_values, max_iterations);
		kmeans.run(points);
		//<<kClusters[i].getId()<<" Has SSE: "<<kClusters[i].getSSE()<<endl;
	}
	cout<<" MIN SSE : "<<minSSE<<" with id : "<<idMinSSE<<endl;	
	cout<<"Cluster of MINSSE is "<<kClusters[idMinSSE].getId()<<endl;
	for(int i = 0 ; i < K ; i ++ ){
		int finalLength = kClusters[idMinSSE].getCluster(i).getTotalPoints();
			cout << "Cluster " << kClusters[idMinSSE].getCluster(i).getID() + 1 << endl;
			//output << "Cluster " << kClusters[idMinSSE].getCluster(i).getID() + 1 << endl; //xu ly doc file
			for(int j = 0; j < finalLength; j++)
			{
				cout << "Point " << kClusters[idMinSSE].getCluster(i).getPoint(j).getID() + 1 << ": ";
				//output << "Point " << kClusters[idMinSSE].getCluster(i).getPoint(j).getID() + 1 << ": ";
				for(int p = 0; p < total_values; p++){
					cout << kClusters[idMinSSE].getCluster(i).getPoint(j).getValue(p) << " ";
					//output << clusters[i].getPoint(j).getValue(p) << " "; //xu ly doc file
				}
				cout << endl;
				//output << endl;
			}

			cout << "Cluster values: ";
			//output << "Cluster values: ";
			for(int j = 0; j < total_values; j++){
					cout << kClusters[idMinSSE].getCluster(i).getCentralValue(j) << " ";
					//output << kClusters[idMinSSE].getCluster(i).getCentralValue(j) << " ";//xu ly doc file
			}
				//output<< endl << endl;
				cout << "\n\n";
		
	}
	cout<<"______END_____";
	return 0;
}
