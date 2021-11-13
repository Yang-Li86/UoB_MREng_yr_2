`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: University of Birmingham
// Engineer: Yang Li (1962731)
// 
// Create Date: 2020/11/10 19:45:35
// Design Name: Synchronous Finite State Machine for DEEM Assignment 2
// Module Name: DEEM_Source_3
// Project Name: DEEM Assignment 2
// Target Devices: BASYS 3 from DIGILENT
// Tool Versions: Vivado 2020.1
//
// Revision: 2020/11/15
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module DEEM_Source_3(Qa, Qb, Qc, Qd, Z, Clk_Q, JAM_A, JAM_B, JAM_C, JAM_D, JAM_Enable, Clk_Set, Clk_Reset);
    output reg Qa;
    output reg Qb;
    output reg Qc;
    output reg Qd;
    output Z;
    output Clk_Q;
    input JAM_A;
    input JAM_B;
    input JAM_C;
    input JAM_D;
    input JAM_Enable;
    input Clk_Set;
    input Clk_Reset;
    
    assign Clk_Q = (Clk_Set & ~Clk_Reset) | (~Clk_Reset & Clk_Q);
    assign   Z   =  ~Qd & ~Qc & Qb & ~Qa;
    
    always @(posedge Clk_Q)
    begin
       Qa <= (~Qc & Qb & ~JAM_Enable) | (Qd & ~Qa & ~JAM_Enable) | (JAM_A & JAM_Enable);
       Qb <= (~Qd & ~Qa & ~JAM_Enable) | (~Qd & ~Qc & ~Qb & ~JAM_Enable) | (Qd & Qa & ~JAM_Enable) | (~Qc & Qb & ~Qa & ~JAM_Enable) | (JAM_B & JAM_Enable);
       Qc <= (Qc & ~Qb & ~JAM_Enable) | (Qd & Qc & ~JAM_Enable) | (Qd & Qa & ~JAM_Enable) | (JAM_C & JAM_Enable);
       Qd <= (~Qb & ~Qa & ~JAM_Enable) | (~Qc & ~Qb & ~JAM_Enable) | (~Qd & Qc & Qa & ~JAM_Enable) | (Qd & ~Qc & ~Qa & ~JAM_Enable) | (JAM_D & JAM_Enable);
     end
endmodule
