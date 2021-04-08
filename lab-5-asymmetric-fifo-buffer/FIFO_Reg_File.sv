`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Dr. Salah Eddin
//////////////////////////////////////////////////////////////////////////////////

module FIFO_Reg_File
    #(parameter ADDR_WIDTH = 3, DATA_WIDTH = 8)
    (
        input logic clk,
        input logic w_en,
        
        // each word needs 2 addresses (top half : bottom half)
        // --> 2 times the address rows needed
        input logic [ADDR_WIDTH : 0] w_addr, 
        input logic [ADDR_WIDTH : 0] r_addr,
        
        // 2 times the size of the read port
        input logic [2 * DATA_WIDTH - 1: 0] w_data, 
        
        output logic [DATA_WIDTH - 1: 0] r_data
    );
    
    // signal declaration
    logic [DATA_WIDTH - 1: 0] memory [0: (2 * (2 ** ADDR_WIDTH)) - 1];
    
    // write operation
    always_ff @(posedge clk)
    begin
        if (w_en)
        begin
            // write top half of 16 bit data first
            memory[w_addr] <= w_data[(2 * DATA_WIDTH - 1) : DATA_WIDTH];
            
            // write bottom half of 16 bit data second at the next address
            memory[w_addr + 1] <= w_data[(DATA_WIDTH - 1) : 0];
        end
    end
            
    // read operation
    assign r_data = memory[r_addr];
    
endmodule
