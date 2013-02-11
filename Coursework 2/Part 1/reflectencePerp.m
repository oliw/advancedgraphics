function [ result ] = reflectencePerp( angle, n1,n2 )
%REFLECTENCE Summary of this function goes here
%   Detailed explanation goes here

angle = (angle/180)*pi;
top = n1*cos(angle) - n2* sqrt(1-(((n1/n2)*sin(angle))^2));
bottom = n1*cos(angle) +n2* sqrt(1-(((n1/n2)*sin(angle))^2));
result = (abs(top/bottom))^2;

end

