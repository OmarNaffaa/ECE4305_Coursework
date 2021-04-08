module chu_blinkled
   #(parameter isSimulation = 1'b0)  // width of output port
   (
    input  logic clk,
    input  logic reset,
    // slot interface
    input  logic cs,
    input  logic read,
    input  logic write,
    input  logic [4:0] addr,
    input  logic [31:0] wr_data,
    output logic [31:0] rd_data,
    // external port    
    output logic [3:0] dout
   );
   
   localparam counterWidth = 16;
   localparam numBlinkLeds = 4;
   localparam numBitsBufAddr = $clog2(numBlinkLeds);
    
   // declaration
   logic [counterWidth - 1:0] bufCountVal [0 : numBlinkLeds - 1]; //value of period in ms
   logic [numBitsBufAddr - 1: 0] bufAddr; //addr into aove array
   logic wr_en;
   
   //only need the lowest 2 bits of th addr line
   assign bufAddr = addr[numBitsBufAddr - 1: 0];

   // body
   // output buffer register
   always_ff @(posedge clk, posedge reset)
      if (reset)
         for(integer i = 0; i < numBlinkLeds; i = i + 1) bufCountVal[i] <= 'b0;
      else   
         if (wr_en)
            bufCountVal[bufAddr] <= wr_data[counterWidth-1:0];
            
   // decoding logic 
   assign wr_en = cs && write;
   // slot read interface
   assign rd_data =  0;
   
   logic clk1KHzEn;
   Clk1KHzEnGen #(.isSimulation(isSimulation)) uClk1KHzEnGen 
   (
        .clk(clk), 
        .reset(reset), 
        .clk1KHzEn(clk1KHzEn)
    );
    
    genvar ledCount;
    generate 
        for( ledCount = 0; ledCount < numBlinkLeds; ledCount = ledCount + 1) begin
            ledCtrl uLedCtrl(
                .*,
                .countVal(bufCountVal[ledCount]),
                .ledOut(dout[ledCount])
            );
        end
    endgenerate

       
endmodule
       




