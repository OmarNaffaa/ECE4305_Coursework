`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa
//////////////////////////////////////////////////////////////////////////////////

module FIFO
    #(parameter ADDR_WIDTH = 3, DATA_WIDTH = 8)
    (
        input logic clk, reset,
        input logic wr, rd,
        input logic [2 * DATA_WIDTH - 1: 0] w_data, // 2 times the size of the read port
        output logic [DATA_WIDTH - 1: 0] r_data,
        output logic full, empty
    );
    
    // signal declaration
    /*
        declare addresses with twice as many address rows since each word is written in
        2 rows in this modification
    */
    logic [ADDR_WIDTH : 0] w_addr, r_addr;
    
    // instantiate fifo controller
    FIFO_Control #(.ADDR_WIDTH(ADDR_WIDTH))
        ctrl_unit (.*);
    
    // instantiate register file
    FIFO_Reg_File #(.ADDR_WIDTH(ADDR_WIDTH), .DATA_WIDTH(DATA_WIDTH))
        r_file_unit (.w_en( wr & ~full), .*); 
        
endmodule
