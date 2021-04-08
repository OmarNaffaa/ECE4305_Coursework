`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2020 04:51:47 PM
// Design Name: 
// Module Name: blinkLedTB
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


module blinkLedTB();
    logic [3:0] led;
    
    logic clk;
    logic reset;
    logic cs;
    logic read;
    logic write;
    logic [4:0] addr;
    logic [31:0] wr_data;
    logic [31:0] rd_data;
    
    chu_blinkled #(.isSimulation(1'b1)) uBlinkLed(.*, .dout(led));
    
    initial clk = 0;
    always #1 clk = ~clk;
     
    initial begin
        reset = 1'b1;
        #15
        reset = 1'b0;
    end
    
    initial begin
        cs = 1'b0;
        #20
        cs = 1'b1;
        write = 1'b1;
        @(negedge clk);
        addr = 4'h0;
        wr_data = 10;
        @(negedge clk);
        addr = 4'h1;
        wr_data = 20;
        @(negedge clk);
        addr = 4'h2;
        wr_data = 40;
        @(negedge clk);
        addr = 4'h3;
        wr_data = 80;
        @(negedge clk);
    end
    
endmodule
