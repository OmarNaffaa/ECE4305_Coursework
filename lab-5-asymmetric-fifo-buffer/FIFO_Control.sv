`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa
//////////////////////////////////////////////////////////////////////////////////

module FIFO_Control
    #(parameter ADDR_WIDTH = 3)
    (
        input logic clk, reset,
        input logic wr, rd,
        output logic full, empty,
        
        // each word needs 2 addresses (top half : bottom half)
        // --> 2 times the address rows needed
        output logic [ADDR_WIDTH : 0] w_addr,
        output logic [ADDR_WIDTH : 0] r_addr
    );
    
    logic [ADDR_WIDTH : 0] wr_ptr, wr_ptr_next;
    logic [ADDR_WIDTH : 0] rd_ptr, rd_ptr_next;
    
    logic full_next;
    logic empty_next;
    
    // registers for status and read/write pointers
    always_ff @(posedge clk, posedge reset)
    begin
        if(reset)
        begin
            wr_ptr <= 0;
            rd_ptr <= 0;
            full <= 1'b0;
            empty <= 1'b1;
        end
        else
        begin
            wr_ptr <= wr_ptr_next;
            rd_ptr <= rd_ptr_next;
            full <= full_next;
            empty <= empty_next;
        end
    end
    
    always_comb
    begin
        // default is to keep old (current) values
        wr_ptr_next = wr_ptr;
        rd_ptr_next = rd_ptr;
        full_next = full;
        empty_next = empty;
        
        unique case({wr, rd})
            2'b01: // read - unchanged since each row in register is the size of the read port
            begin
                if (~empty)
                begin
                    rd_ptr_next = rd_ptr + 1;
                    full_next = 1'b0;
                    if (rd_ptr_next == wr_ptr) // when reading the last stored value
                        empty_next = 1'b1;
                end                
            end
            2'b10: // write - increment by 2 since each 16 bit entry is written to 2 rows
            begin
                if (~full)
                begin
                    wr_ptr_next = wr_ptr + 2;
                    empty_next = 1'b0;
                    if (wr_ptr_next == rd_ptr || wr_ptr_next == rd_ptr - 1) // 2 spots need to be clear for the word
                        full_next = 1'b1;
                end
            end
            2'b11: // read and write simultaneously 
            begin
                empty_next = 1'b0; // this case will never lead to empty since it writes twice and reads once
                
                // do not read or write unless the FIFO is not full
                if(~full)
                begin
                    // must move pointer to new address everytime for asymmetrical FIFO to avoid overwrite
                    wr_ptr_next = wr_ptr + 2;  
                    
                    if (~empty)                     // only increment if there are valid values to read
                        rd_ptr_next = rd_ptr + 1;
                    else
                        rd_ptr_next = rd_ptr;
                    
                    if (wr_ptr_next == rd_ptr) // check with symmetric method since read and write pointers will only differ by 1
                        full_next = 1'b1;
                end
            end
            default: ; // case 2'b00; null statements; no op
        endcase       
    end
    
    // outputs
    assign w_addr = wr_ptr;
    assign r_addr = rd_ptr;
    
endmodule
