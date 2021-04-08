`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa, Connor Carr
//////////////////////////////////////////////////////////////////////////////////

module SquareWaveGen_TB();

    // signal declaration
    localparam B = 4;
    localparam T = 10; // clock period (10 ns)
    
    logic CLK, RST, waveOut;
    logic [B-1:0] M, N;
    
    // instantiate UUT
    SquareWaveGen #(.B(B)) uut (.*);
    
    // generate clock
    always begin
        CLK = 1'b1;
        #(T / 2);
        CLK = 1'b0;
        #(T / 2);
    end
    
    // test cases
    initial begin
        // perform reset
        RST = 1'b1;
        @(negedge CLK)
        RST = 1'b0;
        
        // assign arbitrary values for M and N
        M = 4'b0010;
        N = 4'b0001;
    end
    
endmodule
