`timescale 1ns / 1ps


module BarrelShifter_08b_V1
    (
        input logic [7 : 0] in,
        input logic [2 : 0] amt,
        input logic lr,
        output logic [7 : 0] out
    );
    logic [3:0][7:0] rotateR, rotateL;
    
    assign rotateR[0] = in;
    assign rotateL[0] = in;
    genvar i;
    generate
        for(i = 0; i < 3; i = i + 1) begin
            assign rotateR[i + 1] = (amt[i] == 0) ? rotateR[i] : {rotateR[i][2**i - 1:0],rotateR[i][7 : 2**i]};
            assign rotateL[i + 1] = (amt[i] == 0) ? rotateL[i] : {rotateL[i][7 - 2**i:0],rotateL[i][7 : 7 - 2**i + 1]};
        end
    endgenerate
    
    assign out = lr ? rotateL[3] : rotateR[3];
    
endmodule

module BarrelShifter_08b_V2
    (
        input logic [7 : 0] in,
        input logic [2 : 0] amt,
        input logic lr,
        output logic [7 : 0] out
    );
    
    logic [7:0] rev_or_not;
    logic [3:0][7:0] rotateR;
    
    assign rev_or_not = lr ? {in[0],in[1],in[2],in[3],in[4],in[5],in[6],in[7]} : in;
    
    assign rotateR[0] = rev_or_not;
    genvar i;
    generate
        for(i = 0; i < 3; i = i + 1) begin
            assign rotateR[i + 1] = (amt[i] == 0) ? rotateR[i] : {rotateR[i][2**i - 1: 0],rotateR[i][7 : 2**i]};
        end
    endgenerate
    
    assign out = lr ? {rotateR[3][0],rotateR[3][1],rotateR[3][2],rotateR[3][3],rotateR[3][4],rotateR[3][5],rotateR[3][6],rotateR[3][7]} : rotateR[3];    
    
endmodule

module BarrelShifter_16b_V1
    (
        input logic [15 : 0] in,
        input logic [3 : 0] amt,
        input logic lr,
        output logic [15 : 0] out
    );
    logic [4:0][15:0] rotateR, rotateL;
    
    assign rotateR[0] = in;
    assign rotateL[0] = in;
    genvar i;
    generate
        for(i = 0; i < 4; i = i + 1) begin
            assign rotateR[i + 1] = (amt[i] == 0) ? rotateR[i] : {rotateR[i][2**i - 1:0],rotateR[i][15 : 2**i]};
            assign rotateL[i + 1] = (amt[i] == 0) ? rotateL[i] : {rotateL[i][15 - 2**i:0],rotateL[i][15 : 15 - 2**i + 1]};
        end
    endgenerate
    
    assign out = lr ? rotateL[4] : rotateR[4];
    
endmodule

module BarrelShifter_16b_V2
    (
        input logic [15 : 0] in,
        input logic [3 : 0] amt,
        input logic lr,
        output logic [15 : 0] out
    );
    
    logic [15:0] rev_or_not;
    logic [4:0][15:0] rotateR;
    
    assign rev_or_not = lr ? {	in[0],in[1],in[2],in[3],in[4],in[5],in[6],in[7],in[8],in[9],in[10],in[11],in[12],in[13],in[14],in[15]}
							: in;
    
    assign rotateR[0] = rev_or_not;
    genvar i;
    generate
        for(i = 0; i < 4; i = i + 1) begin
            assign rotateR[i + 1] = (amt[i] == 0) ? rotateR[i] : {rotateR[i][2**i - 1: 0],rotateR[i][15 : 2**i]};
        end
    endgenerate
    
    assign out = lr ? {rotateR[4][0],rotateR[4][1],rotateR[4][2],rotateR[4][3],rotateR[4][4],rotateR[4][5],rotateR[4][6],rotateR[4][7],
    rotateR[4][8],rotateR[4][9],rotateR[4][10],rotateR[4][11],rotateR[4][12],rotateR[4][13],rotateR[4][14],rotateR[4][15]}
					:  rotateR[4];    
    
endmodule

module BarrelShifter_32b_V1
    (
        input logic [31 : 0] in,
        input logic [4 : 0] amt,
        input logic lr,
        output logic [31 : 0] out
    );
    logic [5:0][31:0] rotateR, rotateL;
    
    assign rotateR[0] = in;
    assign rotateL[0] = in;
    genvar i;
    generate
        for(i = 0; i < 5; i = i + 1) begin
            assign rotateR[i + 1] = (amt[i] == 0) ? rotateR[i] : {rotateR[i][2**i - 1:0],rotateR[i][31 : 2**i]};
            assign rotateL[i + 1] = (amt[i] == 0) ? rotateL[i] : {rotateL[i][31 - 2**i:0],rotateL[i][31 : 31 - 2**i + 1]};
        end
    endgenerate
    
    assign out = lr ? rotateL[5] : rotateR[5];
    
endmodule

module BarrelShifter_32b_V2
    (
        input logic [31 : 0] in,
        input logic [4 : 0] amt,
        input logic lr,
        output logic [31 : 0] out
    );
    
    logic [31:0] rev_or_not;
    logic [5:0][31:0] rotateR;
    
    assign rev_or_not = lr ? {	in[0],in[1],in[2],in[3],in[4],in[5],in[6],in[7],in[8],in[9],in[10],in[11],in[12],in[13],in[14],in[15],
								in[16],in[17],in[18],in[19],in[20],in[21],in[22],in[23],in[24],in[25],in[26],in[27],in[28],in[29],in[30],
								in[31]}
							: in;
    
    assign rotateR[0] = rev_or_not;
    genvar i;
    generate
        for(i = 0; i < 5; i = i + 1) begin
            assign rotateR[i + 1] = (amt[i] == 0) ? rotateR[i] : {rotateR[i][2**i - 1: 0],rotateR[i][31 : 2**i]};
        end
    endgenerate
    
    assign out = lr ? {rotateR[5][0],rotateR[5][1],rotateR[5][2],rotateR[5][3],rotateR[5][4],rotateR[5][5],rotateR[5][6],rotateR[5][7],
    rotateR[5][8],rotateR[5][9],rotateR[5][10],rotateR[5][11],rotateR[5][12],rotateR[5][13],rotateR[5][15],rotateR[5][15],
	rotateR[5][16],rotateR[5][17],rotateR[5][18],rotateR[5][19],rotateR[5][20],rotateR[5][21],rotateR[5][22],rotateR[5][23],
    rotateR[5][24],rotateR[5][25],rotateR[5][26],rotateR[5][27],rotateR[5][28],rotateR[5][29],rotateR[5][30],rotateR[5][31]}
					:  rotateR[5];    
    
endmodule