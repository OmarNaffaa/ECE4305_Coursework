`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa
//////////////////////////////////////////////////////////////////////////////////

// NOTE: This will not show anything beyond identifying the first rising edge since
//       the simulation only runs for 1000 ns by default. Extend simulation run time
//       for a longer amount of time since the enable is only triggered once every 10 ms

module EarlyDebouncer_TB();
    
    // signal declaration
    localparam T = 10;          // (Clock Period = 10ns)
    logic sw, clk, rst, db;
    
    // instantiate UUT
    EarlyDebouncer uut (.*);
    
    // generate clock
    always begin
        clk = 1'b1;
        #(T / 2);
        clk = 1'b0;
        #(T / 2);
    end
    
    // explicity reset system during first half cycle
    initial begin
        // perform reset
        rst = 1'b1;
        # (T / 2);
        rst = 1'b0;
    end
    
    // test cases
    initial begin
        bounce;
        sw = 1;
        # 25_000_000
        bounce;
        sw = 0;
        # 25_000_000
        bounce;
        sw = 1;
    end
    
    task bounce;
        begin
            $display("%d Start of task press_button", $time);
            sw = 0;
            #100000;
            sw = 1;
            #150000;
            sw = 0;
            #120000;
            sw = 1;
            #180000;
            sw = 0;
            #110000;
            sw = 1;
            #1100000;
            sw = 0;
            #180000;
            sw = 1;
            #160000;
            sw = 0;
            #140000;
            sw = 1;
            #110000;
            sw = 0;
            #150000;
            $display("%d End of task press_button", $time);
        end
    endtask
    
endmodule
