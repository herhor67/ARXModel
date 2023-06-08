close all
clear variables
clc

n = 140;
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
plot(out.gen.Time, out.gen.Data, 'b-', 'LineWidth', 1.5)
% stem(out.err.Time, out.err.Data)
% stem(out.ster.Time, out.ster.Data)
plot(out.arx.Time, out.arx.Data, 'r-', 'LineWidth', 1.5)
grid on;
% legend(["gen", "err", "ster", "arx"])
xlabel('Iteracja');
ylabel('ARX');
legend('Setp', 'ARX')
title('Wykres wartości ARX w zależności od iteracji');


It = 0:140;

% Wczytanie pliku CSV
data = readmatrix('out.csv', 'Delimiter', ',');

% Pobranie wartości pierwszych i ostatnich kolumn (Step i ARX)
step = data(:, 2);
arx = data(:, end);

% Tworzenie wykresu
figure
hold on
plot(It, step, 'b-', 'LineWidth', 1.5) % Wykres wartości Step
plot(It, arx, 'r-', 'LineWidth', 1.5) % Wykres wartości ARX
grid on;
xlabel('Iteracja');
ylabel('ARX');
legend('Setp', 'ARX')
title('Wykres wartości ARX w zależności od iteracji');

hold off;

% Porównanie wartości ARX z programu i z pliku CSV w postaci tabeli
T = table(It', out.arx.Data, step, arx, 'VariableNames', {'Iteracja', 'ARX z programu', 'Step z pliku', 'ARX z pliku'});
disp(T);
