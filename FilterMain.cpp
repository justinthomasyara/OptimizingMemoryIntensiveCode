#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include "Filter.h"

using namespace std;

#include "rdtsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int
main(int argc, char **argv)
{

  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

    Filter *filter = readFilter(filtername);
//
    double sum = 0.0;
    int samples = 0;

  // change: restructuring the loop
  // initializing structs and variable before the loop
    struct cs1300bmp *input = new struct cs1300bmp; //this was in the for loop
    struct cs1300bmp *output = new struct cs1300bmp; //this was in the for loop
    double sample; //this was in the for loop, declaring it as a double outside the for loop
    
    for (int inNum = 2; inNum < argc; inNum++) {
        string inputFilename = argv[inNum];
        string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    
    // Changing to directly calling the function
    if (cs1300bmp_readfile( (char *) inputFilename.c_str(), input)) {
        sample = applyFilter(filter, input, output); //declared it as a double outside the for loop
        sum = sum + sample; //changed from sum+= sample;
        samples++;
        cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
  }
    fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}
//previously class Filter
struct Filter * //makes the member variables and methods public , made it struct from class
readFilter(string filename)
{
    ifstream input(filename.c_str());
    if ( ! input.bad() ) {
        int size = 0;
        input >> size;
        Filter *filter = new Filter(size);
        int div;
        input >> div;
        filter -> setDivisor(div);
        for (int i=0; i < size; i++) {
            for (int j=0; j < size; j++) {
                int value;
                input >> value;
                filter -> set(i,j,value);
            }
        }
        return filter;
    }
    else {
        cerr << "Bad input in readFilter:" << filename << endl;
        exit(-1);
    }
}

// change: completely restructuring apply filter function
double applyFilter(struct Filter *filter, cs1300bmp *input, cs1300bmp *output)
{
    long long cycStart, cycStop;
    cycStart = rdtscll();
    output -> height = input -> height;
    output -> width = input -> width;
   // initializing variables and function calls before the loop
    int varHeight = (input -> height) - 1; //the for loops were using (input -> height) - 1;
  //for every computation, we can calculate that outside of the loop and store that
  //in memory 
    int varWidth = (input -> width) - 1; //this was also a perimeter for a for loop, so i 
  //assigned it to an outside variable
    int size = filter -> getSize(); //this was also used as a perimeter, we move it outside the for loop
    int divisor = filter -> getDivisor(); //declar it outside the loop
    
  // initializing integer values for pictures before the loop
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
    int val4 = 0;
    int val5 = 0;
    int val6 = 0;
    int val7 = 0;
    int val8 = 0;
    int val9 = 0;
    
  // initializing temporary integer values before the loop
    int temp1 = 0;
    int temp2 = 0;
    int temp3 = 0;
    int temp4 = 0;
    int temp5 = 0;
    int temp6 = 0;
    int temp7 = 0;
    int temp8 = 0;
    int temp9 = 0;

  // initializing an array for x and y values
    int array[size][size]; //the array was used to store the output in the for loop, declaring it 
  //every single time the loop started

  // handling x and y values before the main loop
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            array[i][j] = filter -> get(i, j);
        }
    }

  // changing loop to handle each value while in the current struct 
  // loop also handling two columns and rows each iteration
    for(int r = 1; r <= varHeight; r = r + 2){
        for(int c = 1; c <= varWidth; c = c + 2){
            val1 = 0;
            val2 = 0;
            val3 = 0;
            val4 = 0;
            val5 = 0;
            val6 = 0;
            val7 = 0;
            val8 = 0;
            val9 = 0;     
            for (int i = 0; i < size; i++){ // changing the order of variables
                int row1 = r - 1 + i;// initializing variables before the loop
                int row2 = row1 + 1;
                temp1 = 0;
                temp2 = 0;
                temp3 = 0;
                temp4 = 0;
                temp5 = 0;
                temp6 = 0;
                temp7 = 0;
                temp8 = 0;
                temp9 = 0;
                for (int j = 0; j < size; j++){
                    int col1 = c + j - 1;// initializing variables
                    int col2 = c + 1;
                    int array1 = array[i][j];
                    temp1 = array1 * (temp1 + input -> color[0][row1][col1]);// switching out the loop to handle  
                    temp2 = array1 * (temp2 + input -> color[1][row1][col1]); //each value separately
                    temp3 = array1 * (temp3 + input -> color[2][row1][col1]);
                    temp4 = array1 * (temp4 + input -> color[0][row1][col2]);
                    temp5 = array1 * (temp5 + input -> color[1][row1][col2]); 
                    temp6 = array1 * (temp6 + input -> color[2][row1][col2]);
                    temp7 = array1 * (temp7 + input -> color[0][row2][col1]);
                    temp8 = array1 * (temp8 + input -> color[1][row2][col1]); 
                    temp9 = array1 * (temp9 + input -> color[2][row2][col1]);
                }
                val1 = val1 + temp1;// transferring the sum of the temporary variables to each value
                val2 = val2 + temp2;
                val3 = val3 + temp3;
                val4 = val4 + temp4;
                val5 = val5 + temp5;
                val6 = val6 + temp6;
                val7 = val7 + temp7;
                val8 = val8 + temp8;
                val9 = val9 + temp9;
            }
            val1 = val1 / divisor; // calling getDivisor
            val2 = val2 / divisor;
            val3 = val3 / divisor;
            val4 = val4 / divisor;
            val5 = val5 / divisor;
            val6 = val6 / divisor;
            val7 = val7 / divisor;
            val8 = val8 / divisor;
            val9 = val9 / divisor;
            if (val1 < 0){// making sure the values are in the correct range
                val1 = 0;//we know they have to be between 0 and 255, was given to us in the write up
            }
            if (val1  > 255){ 
                val1 = 255;
            }
            if (val2 < 0){
                val2 = 0;
            }
            if (val2  > 255){ 
                val2 = 255;
            }
            if (val3 < 0){
                val3 = 0;
            }
            if (val3  > 255){ 
                val3 = 255;
            }
            if (val4 < 0){
                val4 = 0;
            }
            if (val4 > 255){ 
                val4 = 255;
            }
            if (val5 < 0){
                val5 = 0;
            }
            if (val5  > 255){ 
                val5 = 255;
            }
            if (val6 < 0){
                val6 = 0;
            }
            if (val6  > 255){ 
                val6 = 255;
            }
            if (val7 < 0){
                val7 = 0;
            }
            if (val7  > 255){ 
                val7 = 255;
            }
            if (val8 < 0){
                val8 = 0;
            }
            if (val8  > 255){ 
                val8 = 255;
            }
            if (val9 < 0){
                val9 = 0;
            }
            if (val9 > 255){ 
                val9 = 255;
            }
            output -> color[0][r][c] = val1; // assigning the values to their output
            output -> color[1][r][c] = val2;
            output -> color[2][r][c] = val3;
            output -> color[0][r][c + 1] = val4;
            output -> color[1][r][c + 1] = val5;
            output -> color[2][r][c + 1] = val6;
            output -> color[0][r + 1][c] = val7;
            output -> color[1][r + 1][c] = val8;
            output -> color[2][r + 1][c] = val9;
        }
    }
    cycStop = rdtscll();
    double diff = cycStop - cycStart;
    double diffPerPixel = diff / (output -> width * output -> height);
    fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
            diff, diff / (output -> width * output -> height));
    return diffPerPixel;
}
