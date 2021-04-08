`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa, Connor Carr
//////////////////////////////////////////////////////////////////////////////////


module slowerClkGen         // slows clock by a factor of 10
    (
        input logic clk_in,
        input logic resetSW,
        output logic clk_out
    );
    
    logic [2:0] counter;
    
    always_ff @ (posedge clk_in)
    begin
        if(resetSW)
            begin
                counter <= 0;
                clk_out <= 0;
            end
        else
            begin
                counter <= (counter + 1) % 5;
                
                if(counter == 0)
                    clk_out <= ~clk_out;
            end
    end
endmodule
