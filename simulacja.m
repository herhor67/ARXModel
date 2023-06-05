close all
clear variables
clc

n = 100;
st = 1;

% Współczynniki wielomianów A i B
A = [1; 1.2; -0.6; 0.3];
B = [0.5];
nk = 1; % opóźnienie

% Projektowanie regulatora PID z użyciem metody Zieglera-Nicholsa
% Ku = 1.5; % współczynnik krytyczny
% Tu = 6; % okres oscylacji przy krytycznym wzmocnieniu
% Kp = 0.6*Ku;
% Ki = 1.2*Kp/Tu;
% Kd = 0.075*Kp*Tu;

Kp = 0.4050;
Ki = 0.1800;
Kd = 0.9000;

% Kp = 0.14;
% Ki = 1.13;
% Kd = 1.32;

% Kp = 1.2;
% Ki = 2.5;
% Kd = 0.8;

% Wielomian charakterystyczny
char_eq = [1; -A];

% Obliczanie pierwiastków
roots = roots(char_eq);

out = sim('model.slx');

figure
hold on
stem(out.gen.Time, out.gen.Data)
stem(out.err.Time, out.err.Data)
stem(out.ster.Time, out.ster.Data)
stem(out.arx.Time, out.arx.Data)
legend(["gen", "err", "ster", "arx"])
