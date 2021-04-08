`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa
//////////////////////////////////////////////////////////////////////////////////

module EarlyDebouncer
    (
        input logic sw,
        input logic rst,
        input logic clk,    // 100 MHz
        output logic db
    );
    
    // 10 ms m_tick generator
    // assuming clk is 100 MHz (clock period is 10 ns)
    // 10 ms / 10 ns is 10e-3 / 10e-9 which is 1_000_000
    logic m_tick;
    mod_m_counter #(.M(1_000_000)) ticker (.clk(clk), .reset(rst), .max_tick(m_tick));
    
    // declare state types
    typedef enum {zero, wait0_1, wait0_2, wait0_3, one, wait1_1, wait1_2, wait1_3} state_type;
    
    // signal declarations
    state_type state_reg, state_next;
    
    // state register
    always_ff @(posedge clk, posedge rst)
        if(rst)
            state_reg <= zero;
        else
            state_reg <= state_next;
            
    // next-state logic
    always_comb
    begin
        db = 0;
        
        case(state_reg)
            zero:
                if(sw)
                    state_next = wait1_1;
                else
                    state_next = zero;
            wait1_1:
                begin
                    db = 1;
                    
                    if(m_tick)                  
                        state_next = wait1_2;
                    else
                        state_next = wait1_1;
                end
            wait1_2:
                begin
                    db = 1;
                
                    if(m_tick)
                        state_next = wait1_3;
                    else
                        state_next = wait1_2;
                end
            wait1_3:
                begin
                    db = 1;

                    if(sw)
                        state_next = wait1_3;
                    else
                        state_next = wait0_1;
                end
            one:
                begin
                    db = 1;
                    
                    if(sw)
                        state_next = one;
                    else
                        state_next = wait0_1;
                end
            wait0_1:
                if(m_tick)                  
                    state_next = wait0_2;
                else
                    state_next = wait0_1;
            wait0_2:
                if(m_tick)                  
                    state_next = wait0_3;
                else
                    state_next = wait0_2;
            wait0_3:
                if(sw)
                    state_next = wait1_1;
                else
                    state_next = wait0_3;
            default:
                state_next = zero;
        endcase
    end
    
endmodule
