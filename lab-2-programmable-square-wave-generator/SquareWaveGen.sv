`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa, Connor Carr
//////////////////////////////////////////////////////////////////////////////////

module SquareWaveGen
    #(parameter B = 4)
    (
        input logic CLK, RST,
        input logic [B - 1: 0] M, N,
        output logic waveOut = 0
    );
    
    // use clock slower instantiation to slow the clock to 100 ns
    logic clkNew;
    slowerClkGen ClockSlower(.clk_in(CLK), .resetSW(RST), .clk_out(clkNew));

    // ------------------------------ //
    // *** [0] Signal Declaration *** //
    // ------------------------------ //
    logic [B:0] r_next, r_reg;
    
    // --------------------------- //
    // *** [1] Regiser Segment *** //
    // --------------------------- //
    always_ff @(posedge clkNew)
    begin
        if(RST)
            r_reg <= 0;
        else
            r_reg <= r_next;
    end
    
    // ---------------------------- //
    // *** [2] next-state logic *** //
    // ---------------------------- //
    // holds computed value for period (time HIGH + time LOW)
    // this should not need more than the number of bits for 'M' + 1
    logic [B:0] totalCnt;
    
    always_comb
    begin
        totalCnt = M + N;                   // compute the total time for the period
        r_next = (r_reg + 1) % totalCnt;    // count only up to the amount calculated above
    end
    
    // ------------------------ //
    // *** [3] Output logic *** //
    // ------------------------ //
    always_comb
    begin
        if(r_reg <= M - 1)  // output HIGH based on value of 'M'
            waveOut = 1;
        else                // otherwise, output LOW based on 'N'
            waveOut = 0;
    end
endmodule