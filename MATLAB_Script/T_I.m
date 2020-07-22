clear 
clc 

%初始化参数
syms T  b I;
r1=0.037;
r2=0.105; h=1*10^-3; n=3; w=0; niandu=0.24;
% dz=[127.1339 -318.0875 202.0143 40.8614 -0.4536]


% 计算过程
xiaolv= 1;   % 修改效率值，改变B_I的关系,这里暂设置为100%
b=0.5146*I*xiaolv;  %0.5146 是计算来的系数值，和磁阻等有关 b-I 系数
tau = 127.1339*b.^4-318.0875*b.^3+202.0143*b.^2+40.8614*b-0.4536;
T=4/3*pi*n*tau*1000*(r2^3-r1^3)+pi*n*niandu*w/h*(r2^4-r1^4);


%标签设置
ezplot(T,[0,2]);

hold on;
T = 539.5683 * I  % k= 539.5683
s = ezplot(T,[0,1.2])
set(s,'linestyle','--');

legend('T-I近似拟合曲线','T-I关系仿真曲线')


xlabel("Electric current(I/A)","fontsize",20)  %标签文字大小
ylabel("Braking torque(T/N*m)","fontsize",20)
title("")




%图形的一些设置
axis([0 2 0 1000]);
grid on;
set(gca,'FontSize',20) %legend、坐标轴数字
set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);  %设置线宽
set(gcf,'Position',[500 250 640 360]); %前两个是初始位置坐标，后两个是初始分辨率




