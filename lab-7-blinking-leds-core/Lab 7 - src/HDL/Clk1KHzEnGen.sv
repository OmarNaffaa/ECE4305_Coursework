`timescale 1ns / 1ps

//////////////////////////////////////////////////////////////////////////////////
// Engineer: Connor Carr
// Module Name: ClkGen
//////////////////////////////////////////////////////////////////////////////////


module Clk1KHzEnGen
    #(parameter isSimulation = 1'b0, maxCount = isSimulation ? 9 : 99999)
    (
        input logic clk,
        input logic reset,
        output logic clk1KHzEn
    );
        
    logic [$clog2(maxCount) - 1 : 0] clkCounter;
    
    always_ff @(posedge clk)
    begin
        if(reset)
            clkCounter <= 0;
        else begin
            if(clkCounter < maxCount)
                clkCounter <= clkCounter + 1;
            else
                clkCounter <= 0;
        end
    end
    
    always_ff @(posedge clk)
    begin
        if(reset)
            clk1KHzEn <= 0;
        else if(clkCounter == maxCount)
            clk1KHzEn <= 1;
        else
            clk1KHzEn <= 0;
    end
endmodule
