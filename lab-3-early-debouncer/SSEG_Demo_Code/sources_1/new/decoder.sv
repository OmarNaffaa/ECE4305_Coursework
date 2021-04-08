`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Dr. Salah Eddin
//////////////////////////////////////////////////////////////////////////////////

module decoder #(parameter N = 3)(
    input logic [N - 1:0] in,
    input logic enable,
    output logic [2**N - 1:0] an
    );
    
    always_comb
    begin
        an = {2**N{1'b1}}; 
        
        if(enable)
        begin
            integer i;
            for(i = 0; i < 2**N; i = i +1)
            begin
                if (in == i)
                    an[i] = 1'b0;
            end
        end
        

    end
    
endmodule