close all
clear variables
clc

n = 100;


% Współczynniki wielomianów A i B
A = [1, 0.2, -0.1, 0.15];
B = [-0.5, 0.6, 0.15];
nk = 1; % opóźnienie

% Projektowanie regulatora PID z użyciem metody Zieglera-Nicholsa
% Ku = 2.2; % współczynnik krytyczny
% Tu = 1.4; % okres oscylacji przy krytycznym wzmocnieniu
% Kp = 0.6*Ku;
% Ti = 0.5*Tu;
% Td = 0.125*Tu;
% Ki = Kp/Ti;
% Kd = Kp*Td;

Kp = 1.32;
Ki = 0.7;
Kd = 0.175;

out = sim('model.slx');

figure
plot(out.arxout)
