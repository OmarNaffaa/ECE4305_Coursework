`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Engineer: Connor Carr
// Module Name: lab5_top
//////////////////////////////////////////////////////////////////////////////////


module lab3_top(
        input logic Clk100MHz, BTNC, BTNU,
        output logic LED
    );

    logic resetN;    
    assign resetN = ~BTNC;
    
    
    EarlyDebouncer uEarlyDebouncer(
        .clk(Clk100MHz),
        .resetN(resetN),
        .inSig(BTNU),
        .outSig(LED)
    );
        
endmodule
