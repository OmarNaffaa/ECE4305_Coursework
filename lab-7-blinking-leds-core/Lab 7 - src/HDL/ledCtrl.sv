`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2020 06:29:24 PM
// Design Name: 
// Module Name: ledCtrl
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module ledCtrl(
    input clk,
    input reset,
    input clk1KHzEn,
    input [15:0] countVal,
    output ledOut
    );
    
    logic [15:0] counterReg;
    always_ff @(posedge clk, posedge reset) begin
        if (reset)
            counterReg <= 0;
        else if(clk1KHzEn) begin
            if(counterReg == countVal - 1)
                counterReg <= 0;          
            else
                counterReg <= counterReg + 1;
        end
    end
    
    logic ledVal;
    always_ff @(posedge clk, posedge reset) begin
        ledVal <= ledVal;
        if (reset)
            ledVal <= 0;
        else if(clk1KHzEn) begin
            if(counterReg == countVal - 1) begin
                if(ledVal == 1'b1)
                    ledVal <= 0;
                else
                    ledVal <= 1;
            end
        end
    end
    assign ledOut = ledVal;
endmodule
