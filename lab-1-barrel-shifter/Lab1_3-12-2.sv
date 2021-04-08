module BarrelShifter
    # (parameter N = 4)         // 2 ^ N = bits for number to shift
    (
        input logic [(2**N) - 1:0] num,
        input logic [N - 1:0] amt,
        output logic [(2**N - 1):0] result
    );
    
    // used to dynamically create an 'N' amount of stages (s0, s1, ... sN)
    logic [(2**N) - 1:0] s [N:0];
    
    // generate shifting 2-1 multiplexer dynamically
    generate
        genvar i;
        
        for(i = 0; i < N; i = i + 1)
        begin
            if(i == 0)
                assign s[0] = amt[0] ? {num[0], num[(2**N)-1:1]} : num;
            else
                assign s[i] = amt[i] ? {s[i-1][(2**i)-1:0], s[i-1][(2**N)-1:(2**i)]} : s[i-1];
        end
    endgenerate    
    
    // assign result to the final stage of the shifter
    assign result = s[N-1];
    
endmodule



`timescale 1 ns / 10 ps

module Testbench();
    // signal declaration
    localparam N = 4;
    logic [(2**N)-1:0] numIn, numOut;
    logic [N-1:0] amt;
    
    // instantiate barrel shifter
    BarrelShifter #(.N(N)) uut (.num(numIn), .amt(amt), .result(numOut));
    
    // generate test values
    initial begin
        numIn = 16'h8888;   // right shift once
        amt = 1;           
        # 1
        numIn = 16'h8888;   // right shift twice
        amt = 2;
        # 1
        numIn = 16'h8888;   // right shift three times
        amt = 3;
        # 1
        numIn = 16'h8888;   // right shift four times
        amt = 4;
        # 1
        $stop;
    end
endmodule
