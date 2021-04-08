`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Engineer: Connor Carr
// Module Name: DownCounter
//////////////////////////////////////////////////////////////////////////////////


module DownCounter
    #(parameter N = 4)
    (
        input logic clk, resetN, countEnable,
        input logic [N - 1:0] maxCount,
        output logic maxTick
    );
    
    // signal declaration
    logic [N - 1:0] r_next, r_reg;
    
    // body
    // [1] Register segment
    always_ff @(posedge clk, negedge resetN)
    begin
        if(resetN == 0 || countEnable == 0)
            r_reg <= 0;
        else
            r_reg <= r_next;
    end
    
    // [2] next-statea logic segment
    assign r_next = (r_reg == (maxCount - 1))? 0: r_reg + 1;
        
    assign maxTick = (r_reg == maxCount - 1) ? 1'b1: 1'b0;
    
endmodule
