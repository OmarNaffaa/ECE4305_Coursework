`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/14/2020 08:40:03 AM
// Design Name: 
// Module Name: EarlyDebouncerFSM
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


module EarlyDebouncerFSM
    (
        input logic clk,
        input logic resetN,
        input logic inSig,
        input logic maxCountReached,
        output logic outSig,
        output logic countEnable
    );
    
    typedef enum {holdLow, waitRisingEdge, holdHigh, waitFallingEdge} state_ty;
    state_ty currentState, nextState;
    
    
    //present state logic
    always_ff @ (posedge clk, negedge resetN)begin
        if (resetN == 0)
            currentState <= holdLow;
        else
            currentState <= nextState;
    end
    
    //next state logic
    always_comb begin
        //default outputs & state
        outSig = 'b0;
        countEnable = 'b0;
        nextState = currentState;
        //state control logic
        case(currentState)
            holdLow: 
                begin
                    if (inSig) begin
                        outSig = 'b1;
                        countEnable = 'b1;
                        nextState = waitRisingEdge;
                    end
                    else begin
                        nextState = holdLow;
                    end
                end
            waitRisingEdge:
            begin
                outSig = 'b1;
                if (maxCountReached == 'b0) begin
                    countEnable = 'b1;
                    nextState = waitRisingEdge;                    
                end
                else begin
                    nextState = holdHigh;
                end
            end
            holdHigh:
            begin
                outSig = 'b1;
                if (inSig) begin
                    nextState = holdHigh;
                end
                else begin
                    countEnable = 'b1;
                    nextState = waitFallingEdge;
                end
            end
            waitFallingEdge:
            begin
                if (maxCountReached == 'b0) begin
                    outSig = 'b1;
                    countEnable = 'b1;
                    nextState = waitFallingEdge;                    
                end
                else begin
                    nextState = holdLow;
                end
            end
            
        endcase
    end
endmodule
