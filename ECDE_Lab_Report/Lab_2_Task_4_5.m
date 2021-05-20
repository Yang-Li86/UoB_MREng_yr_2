close all
clear all

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Yang Li Written on 22/03/2021    %
% Data processing for ECDE lab 2   %
% Task 4 & 5                       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

d = 1e-3 .* [0.8; 1.6; 2.4; 3.2];
V_internal = 1e-3 .* [89.98; 66.22; 46.31; 44.25];
permittivity_of_free_space = 8.85418782e-12;
Area = pi*0.11^2 - pi*0.007^2;
C_pp_theoretical = permittivity_of_free_space*Area ./ d;
V_external = 20;
C_internal = 1e-7;



coeffs = polyfit(1./d(:,1), V_internal(:,1), 1);
d_continuous = 1e-3 .* (0.8:0.1:3.2);
V_continuous = coeffs(1,1) .* (1./d_continuous) + coeffs(1,2);
plot(1./d(:,1), V_internal(:,1), 'o', 1./d_continuous(1,:), V_continuous(1,:));
xlabel('1/d (1/m)');
ylabel('V Internal (V)');

experimental_permittivity_of_free_space = coeffs(1,1)*C_internal/V_external/Area;
C_cable_experimental = coeffs(1,2)*C_internal/V_external;
C_pp_experimental = experimental_permittivity_of_free_space * Area ./ d;


% Task 5
V_internal_with_dielectric_material = 0.13972;
reletive_permittivity = d(1,1)/permittivity_of_free_space/Area * ...
                       (V_internal_with_dielectric_material*C_internal/V_external-...
                       C_cable_experimental);

