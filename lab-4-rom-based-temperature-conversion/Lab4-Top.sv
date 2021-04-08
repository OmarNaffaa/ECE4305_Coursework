`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/22/2020 02:57:23 PM
// Design Name: 
// Module Name: Lab4-Top
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


module Lab4Top(
    input CLK,
    input [8:0] SW, 
    output [7:0] LED
);
    TempConverter uTC(.CLK(CLK), .inTemp(SW[7:0]), .celsiusOrFahr(SW[8]), .outTemp(LED[7:0]));
    
endmodule
