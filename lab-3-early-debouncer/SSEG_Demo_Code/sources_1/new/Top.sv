`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa
//////////////////////////////////////////////////////////////////////////////////

module Top
    (
        input logic sw,
        input logic rst,
        input logic clk, // 100 MHZ
        output logic [6:0] sseg,
        output logic [7:0] an
    );
    
    // instantiate early debouncer
    logic db;
    EarlyDebouncer eDebouncer (.*);
    
    // instantiate edge detectors
    logic swTick, eDebouncerTick;
    
    rising_edge_detect_mealy swEdge (.clk(clk), .reset(rst), .level(sw), .tick(swTick));            // for "raw" switch input
    rising_edge_detect_mealy eDbEdge(.clk(clk), .reset(rst), .level(db), .tick(eDebouncerTick));    // for debounced switch input
    
    // instantiate binary counters
    logic [7:0] swCnt, eDbCnt;
    
    binary_counter swCounter (.q(swCnt), .en(swTick), .max_tick(), .reset(rst), .*);             // for "raw" switch input
    binary_counter eDbCounter(.q(eDbCnt), .en(eDebouncerTick), .max_tick(), .reset(rst), .*);    // for debounced switch input
    
    // instantiate multiplexer display unit
    time_mux_disp disp 
    (
        .in0({1'b1 ,swCnt[3:0], 1'b1}),
        .in1({1'b1 ,swCnt[7:4], 1'b1}),
        .in2(),
        .in3(),
        .in4({1'b1 ,eDbCnt[3:0], 1'b1}),
        .in5({1'b1 ,eDbCnt[7:4], 1'b1}),
        .in6(),
        .in7(),
        .dp(),
        .*
    );
    
endmodule
