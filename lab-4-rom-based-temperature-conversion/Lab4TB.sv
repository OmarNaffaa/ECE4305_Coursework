`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/22/2020 04:21:10 PM
// Design Name: 
// Module Name: Lab4TB
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


module Lab4TB();
    logic CLK;
    logic [7:0] inTemp, outTemp;
    logic celsiusOrFahr;
    
    initial CLK = 0;
    always #1 CLK = ~CLK; 
    
    Lab4Top uTC(
        .CLK(CLK),
        .SW({celsiusOrFahr, inTemp}), 
        .LED(outTemp)
    );
      
    initial inTemp = 0;
    always #1 inTemp = inTemp + 1;
    
    initial begin
        #1;
        celsiusOrFahr = 1'b1;
        inTemp = 0;
        #101;
        inTemp = 30;
        celsiusOrFahr = 1'b0;
        #183;
        $finish;
    end
     
endmodule
