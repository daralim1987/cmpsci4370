#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>

using namespace std;
const int ROW = 3;
const int COL = 2;
int main(int argc, char **argv){
    //Variable declaration
    int xy[ROW][COL];
    int x[ROW];
    int y[ROW];
    
    //Reading from file into 2D array
    ifstream myReadFile;
    myReadFile.open(argv[1]);
    while(!myReadFile.eof()){
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                myReadFile >> xy[i][j];
            }
        }
    }
    //Assign first column to array x and second column to array y
    for(int i = 0; i < COL; i++){
        for(int j = 0; j < ROW; j++){
            if(i == 0){
                x[j] = xy[j][i];
            }
            if(i == 1){
                y[j] = xy[j][i];
            }
        }
    }
    
    //Euclidean distance
    int dist;
    int p1, p2;
    for(int i = 0; i < 3; i++){
        p1 = x[0];
        p2 = y[0];
        if(i >= 1){
            int sum = 0;
            sum = pow((x[i] - p1), 2) + pow((y[i] - p2), 2);
            cout << sum << endl;
        }
        
    }
    
    return 0;
}
