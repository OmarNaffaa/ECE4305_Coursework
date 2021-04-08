#include <iostream>

using namespace std;

// Function that converts decimal to binary 
void decToBinary(int n) 
{ 
    // Size of an integer is assumed to be 8 bits 
    for (int i = 7; i >= 0; i--) { 
        int k = n >> i; 
        if (k & 1) 
            cout << "1"; 
        else
            cout << "0"; 
    } 
} 

// Celsius to Fahrenheit Binary LUT Generator
int main()
{   
    int lowerTemp = 32;
    int higherTemp = 212;
    
    int fahTemp = 0;
    
    for(int i = 0; i < lowerTemp - 1; i++)
        cout << "00000000" << endl;
    
    for(int i = lowerTemp; i <= higherTemp; i++){
        // for fahrenheit
        fahTemp = ((i -32) * 5) / 9;

	    // print 'i' for celcuis in binary        
        decToBinary(fahTemp);

        cout << endl;
    }
    
    return 0;
}
