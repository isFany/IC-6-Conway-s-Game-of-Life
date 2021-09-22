//
//  test_correct.cpp
//
//  Created by Created by acse-cz1620, acse-fy120,acse-xl620 on 17/02/2021.
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
using namespace cv;

/*This cpp is used to test the accuracy of game of life using multiple threads.
The initial state of the concurrent grid is same to non-concurrent grid. After the
same number of iterations, Through comparing each element of grid_parallel with the element
of grid, judge whether the result obtained by multi-threads is correct
*/


vector<vector<bool>> grid, new_grid;

// The size of the both grid and grid_para
int imax = 100;
int jmax = 100;
// The number of iteration
int max_steps = 100;
long sizeArr = imax * jmax;
string teamName = "Team1998";

// creating 1D array for grid_para
bool* grid_para = new bool[sizeArr];
bool* new_grid_para = new bool[sizeArr];

// delete the memory
void deleteArray() {
    if (grid_para != NULL) {
        delete[] grid_para;
        grid_para = NULL;
    }

    if (new_grid_para != NULL) {
        delete[] new_grid_para;
        new_grid_para = NULL;
    }

}

// calculate the alive neighbors of the cell whose position is (ii, jj).
int num_neighbours(int ii, int jj)
{
    // calculate the number of alive neighbors
    int ix, jx;
    int cnt = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (i != 0 || j != 0)
            {
                ix = (i + ii + imax) % imax;
                jx = (j + jj + jmax) % jmax;
                if (grid[ix][jx]) cnt++;
            }
    return cnt;
}

int num_neighbours_para(int ii, int jj)
{
    // calculate the number of alive neighbors
    int ix, jx;
    int cnt = 0;

    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (i != 0 || j != 0)
            {
                ix = (i + ii + imax) % imax;
                jx = (j + jj + jmax) % jmax;
                if (grid_para[ix * jmax + jx]) cnt++;
            }
    return cnt;
}


// This fucntion is to generate the image according to the value of grid value. 
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
    // store the image to the  folder  named img_test
    string filename = "./img_test/" + teamName + " " + to_string(x) + "th " + to_string(imax) + "×" + to_string(jmax) + ".jpg";
    imwrite(filename, m1);
    waitKey(0);
    m1.release();
}


// make a avi file using the jpg in the dir
bool Jpg_To_Video()
{
    if (imax > 1600 || jmax > 1600 || imax != jmax || max_steps < 10) {
        cout << "The size of image or max_steps is not reasonable , can't output video" << endl;
        return false;
    }
    //Create a video
    VideoWriter video("test_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 5, Size(3 * imax, 3 * jmax));
    String rows_string = to_string(imax);
    String cols_string = to_string(jmax);

    //Loop through each jpg and add it to the video
    for (size_t i = 0; i < max_steps; i++)
    {
        String File_Name = "./img_test/" + teamName + " " + to_string(i + 1) + "th " + rows_string + "×" + cols_string + ".jpg";
        // read the jpg
        Mat image = imread(File_Name);
        // Write the picture into the video
        video.write(image);
        image.release();
    }

    video.release();
    return true;
}

// Set the initial state of the cell
// The pattern is special
void pat1()
{
    //First pattern
    grid_para[3 * jmax + 4] = 1;
    grid_para[3 * jmax + 5] = 1;
    grid_para[3 * jmax + 6] = 1;
    grid_para[4 * jmax + 3] = 1;
    grid_para[4 * jmax + 4] = 1;
    grid_para[4 * jmax + 5] = 1;



    // Second pattern
    grid_para[5 * jmax + 20] = 1;
    grid_para[5 * jmax + 21] = 1;
    grid_para[5 * jmax + 22] = 1;
    grid_para[5 * jmax + 26] = 1;
    grid_para[5 * jmax + 27] = 1;
    grid_para[5 * jmax + 28] = 1;

    grid_para[10 * jmax + 20] = 1;
    grid_para[10 * jmax + 21] = 1;
    grid_para[10 * jmax + 22] = 1;
    grid_para[10 * jmax + 26] = 1;
    grid_para[10 * jmax + 27] = 1;
    grid_para[10 * jmax + 28] = 1;

    grid_para[12 * jmax + 20] = 1;
    grid_para[12 * jmax + 21] = 1;
    grid_para[12 * jmax + 22] = 1;
    grid_para[12 * jmax + 26] = 1;
    grid_para[12 * jmax + 27] = 1;
    grid_para[12 * jmax + 28] = 1;



    grid_para[17 * jmax + 20] = 1;
    grid_para[17 * jmax + 21] = 1;
    grid_para[17 * jmax + 22] = 1;
    grid_para[17 * jmax + 26] = 1;
    grid_para[17 * jmax + 27] = 1;
    grid_para[17 * jmax + 28] = 1;

    grid_para[7 * jmax + 18] = 1;
    grid_para[8 * jmax + 18] = 1;
    grid_para[9 * jmax + 18] = 1;
    grid_para[13 * jmax + 18] = 1;
    grid_para[14 * jmax + 18] = 1;
    grid_para[15 * jmax + 18] = 1;

    grid_para[7 * jmax + 23] = 1;
    grid_para[8 * jmax + 23] = 1;
    grid_para[9 * jmax + 23] = 1;
    grid_para[13 * jmax + 23] = 1;
    grid_para[14 * jmax + 23] = 1;
    grid_para[15 * jmax + 23] = 1;

    grid_para[7 * jmax + 25] = 1;
    grid_para[8 * jmax + 25] = 1;
    grid_para[9 * jmax + 25] = 1;
    grid_para[13 * jmax + 25] = 1;
    grid_para[14 * jmax + 25] = 1;
    grid_para[15 * jmax + 25] = 1;

    grid_para[7 * jmax + 30] = 1;
    grid_para[8 * jmax + 30] = 1;
    grid_para[9 * jmax + 30] = 1;
    grid_para[13 * jmax + 30] = 1;
    grid_para[14 * jmax + 30] = 1;
    grid_para[15 * jmax + 30] = 1;


    //Third  pattern
    grid_para[54 * jmax + 50] = 1;
    grid_para[54 * jmax + 51] = 1;
    grid_para[55 * jmax + 50] = 1;

    grid_para[56 * jmax + 53] = 1;
    grid_para[57 * jmax + 53] = 1;
    grid_para[57 * jmax + 52] = 1;


    // Fourth pattern
    grid_para[64 * jmax + 50] = 1;
    grid_para[64 * jmax + 51] = 1;
    grid_para[65 * jmax + 50] = 1;
    grid_para[65 * jmax + 51] = 1;

    // Fifth pattern
    grid_para[0 * jmax + 50] = 1;
    grid_para[1 * jmax + 50] = 1;
    grid_para[2 * jmax + 50] = 1;
    grid_para[1 * jmax + 48] = 1;
    grid_para[2 * jmax + 49] = 1;

}

// get the state of each cell after one iteration
// multiple thread
void do_iteration_para(void)
{
    //Increase the iteration speed by computing the matrix in parallel
    //Because multiple threads modify new_grid at the same time, so shared new_grid
#pragma omp parallel for shared(new_grid_para)
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++)
        {
            new_grid_para[i * jmax + j] = grid_para[i * jmax + j];
            int num_n = num_neighbours_para(i, j);
            if (grid_para[i * jmax + j])
            {
                if (num_n != 2 && num_n != 3)
                    new_grid_para[i * jmax + j] = false;
            }
            else if (num_n == 3) {
                new_grid_para[i * jmax + j] = true;
            }
        }
    //Parallel processing of assignment of one-dimensional arrays
#pragma omp parallel for shared(grid_para)
    for (int i = 0; i < imax * jmax; i++)
    {
        grid_para[i] = new_grid_para[i];
    }
}

// get the state of each cell after one iteration
// single thread
void do_iteration(void)
{
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++)
        {
            new_grid[i][j] = grid[i][j];
            int num_n = num_neighbours(i, j);
            if (grid[i][j])
            {
                if (num_n != 2 && num_n != 3)
                    new_grid[i][j] = false;
            }
            else if (num_n == 3) new_grid[i][j] = true;
        }
    grid.swap(new_grid);
}


// We set the initial state to a random number, which makes the test more reliable
bool testcase1() {
    srand(time(NULL));
    grid.resize(imax, vector<bool>(jmax));
    new_grid.resize(imax, vector<bool>(jmax));
    bool isSame = true;

    // set an initial random collection of points
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++) grid[i][j] = (rand() % 2);

    // Assign the initial state of the grid to grid_para
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++)  grid_para[i * jmax + j] = grid[i][j];

    // do iteration
    for (int n = 0; n < max_steps; n++)
    {
        do_iteration();
        do_iteration_para();
    }

    // Compare each element of grid_paar with grid.
    // If one or more elements are not the same, 
    // it means the result obtaind by multiple threads is wrong 
    for (int i = 0; i < imax; i++)
    {
        for (int j = 0; j < jmax; j++)
        {
            if (grid_para[i * jmax + j] != grid[i][j])
            {
                isSame = false;
            }
        }
    }
    return isSame;
}

//This test is to generate the video to evaluate the correctness of result from multuple threads
bool testcase2() {
    srand(time(NULL));
    // set an initial pattern
#pragma omp parallel for shared(grid_para)
    for (int i = 0; i < imax; i++)
    {
        for (int j = 0; j < jmax; j++)
        {
            //#pragma omp critical
            grid_para[i * jmax + j] = 0;
        }
    }
    if (imax < 100 || jmax < 100) {
        cout << " The imax or jmax is too small, can't add the pattern" << endl;
    }
    else {
        pat1();
    }


    //Iterate and store the result of each iteration
    for (int n = 0; n < max_steps; n++)
    {
        do_iteration_para();
        grid_to_jpg(grid_para, n);
    }
    // generate the vdeio
    bool result = Jpg_To_Video();
    return result;
}

int main(int argc, char* argv[])
{
    if (imax < 1 || jmax < 1 || max_steps < 1) {
        cout << " The size of grid or max_steps is not reasonable, please re-set" << endl;
        exit(0);
    }
    //cout the test result
    cout << "test1 past: " << testcase1() << "\n";

    //testcase2() is run in release mode in vs
    //cout << "test2 past: " << testcase2() << endl;

    //delete the memory
    deleteArray();
    return 0;
}