function [ result ] = foo( )

X = 0:0.1:90;
Y = zeros(1,length(X));
Z = zeros(1,length(X));

for i=1:length(X)
    Y(i) = reflectencePerp(X(i),1,1.5);
    Z(i) = reflectencePar(X(i),1,1.5);
end

msg = sprintf('Reflectance at normal incidence (zero degrees)\n');
disp(msg);
msg = sprintf('S-Polarised: %d \n',Y(1));
disp(msg);
msg = sprintf('P-Polarised: %d \n',Z(1));
disp(msg);

figure;
hold on;
title('From Air To Material');
xlabel('Degrees of incidence');
ylabel('Amount Reflected /1');
a1 = plot(X,Y);
M1 = 'S-Polarised (Perpendicular)';
a2 = plot(X,Z,'r');
M2 = 'P-Polarised (Parallel)';
legend([a1,a2],M1,M2);
hold off;

Y2 = zeros(1,length(X));
Z2 = zeros(1,length(X));

for i=1:length(X)
    Y2(i) = reflectencePerp(X(i),1.5,1);
    Z2(i) = reflectencePar(X(i),1.5,1);
end

figure;
hold on;
title('From Material To Air');
xlabel('Degrees of incidence');
ylabel('Amount Reflected /1');
a1 = plot(X,Y2);
M1 = 'S-Polarised (Perpendicular)';
a2 = plot(X,Z2,'r');
M2 = 'P-Polarised (Parallel)';
legend([a1,a2],M1,M2);
hold off;