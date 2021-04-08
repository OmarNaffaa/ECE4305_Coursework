`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa
//////////////////////////////////////////////////////////////////////////////////

module RomBasedTempConversion_TB();
    
    // signal declaration
    localparam T = 10;      // 10 ns Clock Period
    logic clk, format;
    logic [7:0] temp, data;
    
    // instantiate UUT
    RomBasedTempConversion uut (.*);
    
    // generate clock
    always begin
        clk = 1'b1;
        #(T / 2);
        clk = 1'b0;
        #(T / 2);
    end
    
    // test cases
    initial begin
        format = 1; temp = 212;  // 212 F --> 100 C
        # (2 * T)
        
        temp = 128;              // 128 F --> ~53 C
        # (2 * T)
        
        temp = 136;              // 136 F --> ~57 C
        # (2 * T)
        
        format = 0; temp = 0;    // 0 C --> 32 F
        # (2 * T)
        
        temp = 64;               // 64 C --> ~147 F
        # (2 * T)
        
        $finish;
    end
    
endmodule
