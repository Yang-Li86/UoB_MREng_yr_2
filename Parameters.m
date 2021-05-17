close all;
clear all;

%% Regenerative_Energy_and_Emergency_Power.slx
Battery_nominal_v = 380;      % V
Rated_capacity = 1e+6;        % Ah


%% Light_duty_Brushed_DC_Motor.slx
%  250 W Maxon RE 65 Ø65 mm, Graphite Brushes 388991
%  https://www.maxongroup.com/medias/sys_master/root/8841119432734/EN-143.pdf
Armature_inductance = 0.644;  % mH
Stall_torque = 12200;         % mNm
No_load_speed = 2690;         % rpm
Rated_DC_supply_voltage = 70; % V
No_load_current = 179;        % mA
Rotor_inertia_DC = 1340;         % g*cm^2
Initial_rotor_speed_DC = 0;      % rpm

% Mechanical Load
Torque_Load = 1;              % Nm

%% Heavy_duty_AC_Motor.slx
% three-phase Squirrel-Cage-Motors SIMOTICS
% Motor type : 1CV2282D
% SIMOTICS SD - 280 M - IM V35 - 8p
Rated_mechanical_power = 45;                 % kW
Rated_speed = 738;                           % rpm
Rated_RMS_line_to_line_voltage = 380;        % V
Rated_supply_frequency = 50;                 % Hz
Rated_RMS_line_current = 94;                 % A
Motor_efficiency_percent = 92.8;             % Percent
Number_of_pole_pairs = 4;
Number_of_phases = 3;
Supply_RMS_line_to_line_voltage = 380;       % V
Supply_frequency = 50;                       % Hz
Rotor_inertia = 1.4;                         % kg*m^2
Rotor_damping = 1e-4;                        % I could not find this one, went default
Initial_rotor_speed = 0;                     % rpm