`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa
//////////////////////////////////////////////////////////////////////////////////

module Top
    (
        input logic clk, format,
        input logic [7:0] temp,
        output logic [6:0] sseg,
        output logic [7:0] an
    );
    
    // instantiate ROM based temperature converter
    /*
        if format = 1 --> celsius input, fahrenheit output
        if format = 0 --> fahrenheit input, celsius output
    */
    logic [7:0] data;
    RomBasedTempConversion converterUnit (.*);
    
    // connect output to seven segment top module
    /*
        rightmost ssegs display input temperature
        leftmost ssegs display converted temperature
    */
    time_mux_disp disp 
    (
        .in0({1'b1 ,temp[3:0], 1'b1}),
        .in1({1'b1 ,temp[7:4], 1'b1}),
        .in2(),
        .in3(),
        .in4({1'b1 ,data[3:0], 1'b1}),
        .in5({1'b1 ,data[7:4], 1'b1}),
        .in6(),
        .in7(),
        .dp(),
        .*
    );
endmodule
