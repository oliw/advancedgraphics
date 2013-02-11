function [ result ] = reflectencePar( angle, n1,n2 )
%REFLECTENCEPAR Summary of this function goes here
%   Detailed explanation goes here
angle = (angle/180)*pi;
top = n1* sqrt(1-(((n1/n2)*sin(angle))^2)) - n2*cos(angle);
bottom = n1* sqrt(1-(((n1/n2)*sin(angle))^2)) + n2*cos(angle);
result = (abs(top/bottom))^2;

end

