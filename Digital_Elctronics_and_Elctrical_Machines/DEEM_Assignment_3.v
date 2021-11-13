`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/11/26 11:18:07
// Design Name: 
// Module Name: Asynchronous_FSM_2
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Asynchronous_FSM_2(k0, k1, k2, k3, k4, k5, k6, k7, k8, k9, Dr, Reset, Qc, Qb, Qa, Z);
    input k0;
    input k1;
    input k2;
    input k3;
    input k4;
    input k5;
    input k6;
    input k7;
    input k8;
    input k9;
    input Dr;         //  When Dr=1, the door is open
    input Reset;      //  When Reset=1, the lock sets itself to the initial idling state
    output Qc;
    output Qb;
    output Qa;
    output Z;         //  When Z=1, the solenoid bolt retracts itself
    
    assign Qc = (~Reset & ~k0 & ~k1 & ~k2 & k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9 & ~Qc & Qb & ~Qa)
              | (~Reset & ((~k0 & ~k1 & ~k2 & k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9)|(~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9)) & Qc & Qb & ~Qa)
              | ( Reset & Qc & Qb & ~Qa)
              | ((k0 | k1 | k4 | k5 | k6 | k7 | k8 | k9) & Qc & Qb & ~Qa)
              | (Qc & Qb & Qa)
              | (~Reset & ~k0 & ~k1 & k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9 & Qc & Qb & ~Qa)
              | (~Reset & ~Dr & Qc & ~Qb & ~Qa);
    
    assign Qb = (~Reset & ~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & k9 & ~Qc & ~Qb & Qa)
              | (~Reset & ((~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & k9)|(~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9)) & ~Qc & Qb & Qa)
              | (~Reset & ~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & k6 & ~k7 & ~k8 & ~k9 & ~Qc & Qb & Qa)
              | (~Reset & ((~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & k6 & ~k7 & ~k8 & ~k9)|(~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9)) & ~Qc & Qb & ~Qa)
              | (~Reset & ~k0 & ~k1 & ~k2 & k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9 & ~Qc & Qb & ~Qa)
              | (~Reset & ((~k0 & ~k1 & ~k2 & k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9)|(~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9)) & Qc & Qb & ~Qa)
              | ( Reset & Qc & Qb & ~Qa)
              | ((k0 | k1 | k4 | k5 | k6 | k7 | k8 | k9) & Qc & Qb & ~Qa);
    
    assign Qa = (~Reset & ~k0 & ~k1 & ~k2 & ~k3 & ~k4 & k5 & ~k6 & ~k7 & ~k8 & ~k9 & ~Qc & ~Qb & ~Qa)
              | (~Reset & ((~k0 & ~k1 & ~k2 & ~k3 & ~k4 & k5 & ~k6 & ~k7 & ~k8 & ~k9)|(~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9)) & ~Qc & ~Qb & Qa)
              | (~Reset & ~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & k9 & ~Qc & ~Qb & Qa)
              | (~Reset & ((~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & k9)|(~k0 & ~k1 & ~k2 & ~k3 & ~k4 & ~k5 & ~k6 & ~k7 & ~k8 & ~k9)) & ~Qc & Qb & Qa)
              | ( Reset & Qc & Qb & ~Qa)
              | ((k0 | k1 | k4 | k5 | k6 | k7 | k8 | k9) & Qc & Qb & ~Qa)
              | (Qc & Qb & Qa)
              | ((k0 | k1 | k2 | k3 | k4 | k5 | k7 | k8) & ~Qc & Qb & Qa)
              | ( Reset & ~Qc & Qb & Qa)
              | (Qc & ~Qb & Qa);
    
    assign Z = Qc & ~Qb & ~Qa;
endmodule
