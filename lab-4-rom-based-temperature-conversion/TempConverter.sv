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


module TempConverter(
    input CLK,
    input logic [7:0] inTemp,
    input logic celsiusOrFahr,//1->C 0->F     
    output logic [7:0] outTemp
    );
    
    localparam celsiusMax = 100;
    localparam celsiusMin = 0;
    localparam celsiusLen = celsiusMax - celsiusMin + 1; //inclusive [100 : 0]
    
    localparam fahrenheitMax = 212;
    localparam fahrenheitMin = 32;
    localparam fahrenheitLen = fahrenheitMax - fahrenheitMin + 1; //inclusive [212 : 32]
    
    localparam romLen = fahrenheitLen + celsiusLen; 
    localparam romIndexWidth = $clog2(romLen);
    logic [romIndexWidth:0] romIndex;
        
    assign romIndex = (celsiusOrFahr) ? inTemp : inTemp - fahrenheitMin + celsiusLen;
    
    // rom declaration
    (* rom_style = "block" *) logic [7:0] rom [0 : romLen - 1];
    
    initial begin
        $readmemh("ctof.mem", rom, 0, celsiusLen - 1);
        $readmemh("ftoc.mem", rom, celsiusLen,  romLen - 1);
    end
        
    always_ff @(posedge CLK) begin
            outTemp <= rom[romIndex];
    end

endmodule
