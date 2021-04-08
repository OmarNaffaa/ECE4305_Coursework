`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Dr. Salah Eddin
//////////////////////////////////////////////////////////////////////////////////

module mod_m_counter
    #(parameter M = 10)
    (
        input logic clk, reset,
        output logic max_tick
    );
        
    localparam N = $clog2(M); // N is a constant representing number of necessary bits for the counter
    
    // signal declaration
    logic [N - 1:0] r_next, r_reg;
    
    // body
    // [1] Register segment
    always_ff @(posedge clk, posedge reset)
    begin
        if(reset)
            r_reg <= 0;
        else
            r_reg <= r_next;
    end
    
    // [2] next-state logic segment
    assign r_next = (r_reg == (M - 1))? 0: r_reg + 1;
    
    // [3] output logic segment
    assign max_tick = (r_reg == M - 1) ? 1'b1: 1'b0;
    
endmodule