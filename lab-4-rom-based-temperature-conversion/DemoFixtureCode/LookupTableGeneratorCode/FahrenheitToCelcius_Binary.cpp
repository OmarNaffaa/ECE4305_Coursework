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

// fahrenheit to celcius LUT generation
int main()
{   
    int lowerTemp = 0;
    int higherTemp = 100;
    
    int fahTemp = 0;
    
    for(int i = lowerTemp; i <= higherTemp; i++){
        // for fahrenheit
        fahTemp = ((i * 9) / 5) + 32;
      
        decToBinary(fahTemp);
        
        cout << endl;
    }
    
    return 0;
}
