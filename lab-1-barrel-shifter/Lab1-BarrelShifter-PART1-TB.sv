`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Engineer: Connor Carr
// Module Name: BarrelShifter_tb
//////////////////////////////////////////////////////////////////////////////////

module BarrelShifter_tb();
    localparam N = 5;
    logic [2**N - 1 :0] in_tb;
    
    logic [1:0][2**(N) - 1: 0] out32_tb;
    logic [1:0][2**(N - 1) - 1: 0] out16_tb;
    logic [1:0][2**(N - 2) - 1: 0] out8_tb;
    
    logic lr;
    logic [N - 1: 0] amt_tb;
    
    BarrelShifter_08b_V1 brlshft0(          .in(in_tb[7:0]),
                                            .amt(amt_tb[2:0]),
                                            .lr(lr),
                                            .out(out8_tb[0][7:0])
                                            );
                                            
    BarrelShifter_08b_V2 brlshft1(          .in(in_tb[7:0]),
                                            .amt(amt_tb[2:0]),
                                            .lr(lr),
                                            .out(out8_tb[1][7:0])
                                            );
                                            
    BarrelShifter_16b_V1 brlshft2(          .in(in_tb[15:0]),
                                            .amt(amt_tb[3:0]),
                                            .lr(lr),
                                            .out(out16_tb[0][15:0])
                                            );
                                            
    BarrelShifter_16b_V2 brlshft3(          .in(in_tb[15:0]),
                                            .amt(amt_tb[3:0]),
                                            .lr(lr),
                                            .out(out16_tb[1][15:0])
                                            );
                                            
    BarrelShifter_32b_V1 brlshft4(          .in(in_tb[31:0]),
                                            .amt(amt_tb[4:0]),
                                            .lr(lr),
                                            .out(out32_tb[0][31:0])
                                            );
                                            
    BarrelShifter_32b_V2 brlshft5(          .in(in_tb[32:0]),
                                            .amt(amt_tb[4:0]),
                                            .lr(lr),
                                            .out(out32_tb[1][31:0])
                                            );
    
    initial begin
        in_tb = 32'h000AB;
        amt_tb = 5'b00000;
        lr = 1'b1;
        
        for(int i = 0; i < 2**N; i = i + 1) begin
            #10
            amt_tb = amt_tb + 1;
        end
        
        lr = 1'b0;
        
        for(int i = 0; i < 2**N; i = i + 1) begin
            #10
            amt_tb = amt_tb + 1;
        end
    end
endmodule