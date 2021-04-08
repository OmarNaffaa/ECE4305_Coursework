`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona
// Engineer: Omar Naffaa
//////////////////////////////////////////////////////////////////////////////////

module RomBasedTempConversion
    (
        input logic clk,
        input logic format,         // determines whether input is celsius or fahrenheit
        input logic [7:0] temp,     // 8 bits needed enter temperature
        output logic [7:0] data     // 8 bit temperature number will be printed
    );
    
    // signal declaration
    (* rom_style = "block" *) logic [7:0] rom [0:312]; // 8 bit entry x 313 row table for all temperature binary values
                                                       // extra rows added to make finding the temperatures faster with
                                                       // less overhead / computation needed
    
    // read mem files into ROM
    initial
        $readmemb("binary_temp_lut.mem", rom);  // binary_temp_lut is celsius_binary_lut + fahrenheit_binary_lut
    
    // read from ROM based on input
    always_ff @(posedge clk)
        if(format)
            data <= rom[temp + 213]; // format == celsuis to fahrenheit conversion, fahrenheit values are values 213-312
        else
            data <= rom[temp];       // !format == fahrenheit to celsius conversion, celcius values are values 0 - 212
endmodule
