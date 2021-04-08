`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/13/2020 08:44:00 PM
// Design Name: 
// Module Name: EarlyDebouncer
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


module EarlyDebouncer
    #(parameter waitTime_ms = 20)
    (
        input logic clk, resetN,
        input logic inSig,
        output logic outSig
    );
    
    
    localparam maxCount = waitTime_ms * 100;//0000;
    localparam counterSize = $clog2(maxCount);
    
    logic countEnable, maxCountReached;
    DownCounter #(.N(counterSize)) uCounter
    (
        .clk(clk),
        .resetN(resetN),
        .countEnable(countEnable),
        .maxCount(maxCount),
        .maxTick(maxCountReached)
    );
    
    
    EarlyDebouncerFSM uFSM
    (
        .clk(clk),
        .resetN(resetN),
        .inSig(inSig),
        .maxCountReached(maxCountReached),
        .outSig(outSig),
        .countEnable(countEnable)
    );
    
endmodule
