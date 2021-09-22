//
//  ConwaysGame_P_new.cpp
//
//  Created by acse-cz1620, acse-fy120,acse-xl620 on 17/02/2021.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <chrono>
#include <ctime>
#include <omp.h>
#include <vector>
#include <opencv.hpp>

using namespace std;
using namespace chrono;
using namespace cv;


//change the size of grid and total generations
int imax = 1000;
int jmax = 1000;
int max_steps = 100;
long sizeArr = imax * jmax;
string teamName = "Team1998";


//creating 1D array for grid
bool* grid = new bool[sizeArr];
bool* new_grid = new bool[sizeArr];
//creating a vector to store each generation for the final printing
vector<bool*> grid_list;



// delete the memory on heap of grid and new_grid
void deleteArr() {
    if (grid != NULL) {
        delete[] grid;
        grid = NULL;
    }

    if (new_grid != NULL) {
        delete[] new_grid;
        new_grid = NULL;
    }
}



// delete the memory on heap of grid in vector grid_list
void deleteVect() {
    for (int i = 0; i < grid_list.size(); i++) {
        if (grid_list[i] != NULL) {
            delete[] grid_list[i];
            grid_list[i] = NULL;
        }
    }
}



int num_neighbours(int ii, int jj)
{
    //calculate the number of alive neighbors
    int ix, jx;
    int cnt = 0;

    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (i != 0 || j != 0)
            {
                ix = (i + ii + imax) % imax;
                jx = (j + jj + jmax) % jmax;
                if (grid[ix * jmax + jx]) cnt++;
            }
    return cnt;
}



void do_iteration(void)
{
    //Increase the iteration speed by computing the matrix in parallel
    //Because multiple threads modify new_grid at the same time, so shared new_grid
#pragma omp parallel for shared(new_grid)
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++)
        {
            new_grid[i * jmax + j] = grid[i * jmax + j];
            int num_n = num_neighbours(i, j);
            if (grid[i * jmax + j])
            {
                if (num_n != 2 && num_n != 3)
                    new_grid[i * jmax + j] = false;
            }
            else if (num_n == 3) {
                new_grid[i * jmax + j] = true;
            }
        }

    //Parallel processing of assignment of one-dimensional arrays
#pragma omp parallel for shared(grid)
    for (int i = 0; i < imax * jmax; i++)
    {
        //#pragma omp parallel critical
        grid[i] = new_grid[i];
    }
}



void add2list()
{
    //creat a copy of array for storing cell data of current generation
    bool* grid_copy = new bool[sizeArr];
    //assign values to copy grid
#pragma omp parallel for shared(grid_copy)
    for (int i = 0; i < imax * jmax; i++)
    {
        grid_copy[i] = grid[i];
    }
    //add grid of current generation 
    grid_list.push_back(grid_copy);
}



//This fucntion is to generate the image according to the value of grid value. 
void grid_to_jpg(bool grid[], int it) {
    // According to a pixel drawing, the picture will not be clear enough. 
    // Therefore, in the image, we use nine pixels to represent one pixel in the grid
    // The size of m1 is (3*imax,3*jmax)
    Mat m1(3 * imax, 3 * jmax, CV_8UC1);

    // Use multiple threads to draw each pixel
#pragma omp parallel for shared(m1)
    for (int i = 1; i < m1.rows; i += 3)
    {
        //if the value in grid is true, the nine pixels in image is white
       //if the value in grid is false, the nine pixels in image is black
        for (int j = 1; j < m1.cols; j += 3) {
            if (grid[(i - 1) / 3 * jmax + (j - 1) / 3]) {
                m1.ptr<uchar>(i - 1)[j - 1] = 255;
                m1.ptr<uchar>(i - 1)[j] = 255;
                m1.ptr<uchar>(i - 1)[j + 1] = 255;
                m1.ptr<uchar>(i)[j - 1] = 255;
                m1.ptr<uchar>(i)[j] = 255;
                m1.ptr<uchar>(i)[j + 1] = 255;
                m1.ptr<uchar>(i + 1)[j - 1] = 255;
                m1.ptr<uchar>(i + 1)[j] = 255;
                m1.ptr<uchar>(i + 1)[j + 1] = 255;
            }
            else {
                m1.ptr<uchar>(i - 1)[j - 1] = 0;
                m1.ptr<uchar>(i - 1)[j] = 0;
                m1.ptr<uchar>(i - 1)[j + 1] = 0;
                m1.ptr<uchar>(i)[j - 1] = 0;
                m1.ptr<uchar>(i)[j] = 0;
                m1.ptr<uchar>(i)[j + 1] = 0;
                m1.ptr<uchar>(i + 1)[j - 1] = 0;
                m1.ptr<uchar>(i + 1)[j] = 0;
                m1.ptr<uchar>(i + 1)[j + 1] = 0;
            }
        }
    }
    int x = it + 1;
    //store the image to the  folder  named img
    string filename = "./img/" + teamName + " " + to_string(x) + "th " + to_string(imax) + "×" + to_string(jmax) + ".jpg";
    imwrite(filename, m1);
    waitKey(0);
}



inline const char* const BoolToString(bool b)
{
    //Convert boolean value to string for output
    if (b)
    {
        return "1";
    }
    else
    {
        return "0";
    }
}



void grid_to_file(int it)
{
    stringstream fname;
    fstream f1;
    fname << "output" << "_" << it << ".dat";
    f1.open(fname.str().c_str(), ios_base::out);
    //In this part, parallel and merge the values of each row into a string for output.
    //This will reduce the number of calls to f1 and reduce running time
    for (int i = 0; i < imax; i++)
    {
        string temp = "";
        for (int j = 0; j < jmax; j++)
        {
            const char* unit = BoolToString(grid_list[it][i * jmax + j]);
            //Integrate string
            temp = temp + "\t" + string(unit);
        }
        //Print line by line
        f1 << temp << endl;
    }
    f1.close();
}



int main(int argc, char* argv[])
{
    if (imax < 1 || jmax < 1 || max_steps < 1) {
        cout << " The size of grid or max_steps is not reasonable, please re-set" << endl;
        exit(0);
    }
    srand(time(NULL));

    auto start = system_clock::now();

    //set an initial random collection of points - You could set an initial pattern

#pragma omp parallel for shared(grid)
    for (int i = 0; i < imax; i++)
    {
        for (int j = 0; j < jmax; j++)
        {
            //#pragma omp critical
            grid[i * jmax + j] = (rand() % 2);
        }
    }

    //Iterate and store the result of each iteration
    for (int n = 0; n < max_steps; n++)
    {
        cout << "it: " << n << endl;
        do_iteration();
        //we add a copy of grid of current generation and store it in a vector
        add2list();

    }

    //timing the program and print out the time in console.
    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";


    //print the images when game finished in order to prevent our programe from frequently switching
    //between serial and parallelism

    for (int i = 0; i < max_steps; i++)
    {
        if (i == 0) {
            grid_to_jpg(grid_list[i], i);

        }
        else if (i == int(max_steps / 4)) {
            grid_to_jpg(grid_list[i], i);

        }
        else if (i == int(max_steps / 2)) {
            grid_to_jpg(grid_list[i], i);

        }
        else if (i == max_steps - 1) {
            grid_to_jpg(grid_list[i], i);

        }
        //grid_to_file(i);
    }

    deleteArr();
    deleteVect();
    return 0;
}