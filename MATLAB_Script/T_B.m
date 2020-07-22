clear
clc
syms T  
r1=0.037;

%r2=0.075; 
r2 = 0.105;
% 间隙
%h=1.5*10^-3; 
h = 1*10^-3;
%制动盘数目
n=3; 
% 转速 ，rad/s
w=100; 
niandu=0.24;


% dz=[127.1339 -318.0875 202.0143 40.8614 -0.4536]
b =(0:0.05:1.2);
% axis([0 1.2 0 300])
tau = 127.1339*b.^4-318.0875*b.^3+202.0143*b.^2+40.8614*b-0.4536;
T=4/3*pi*n*tau*1000*(r2^3-r1^3)+pi*n*niandu*w/h*(r2^4-r1^4);
plot(b,T,'bo-')
xlabel("Magnetic induction intensity(B/T)","fontsize",20)  %标签文字大小
ylabel("Braking torque(T/N*m)","fontsize",20)

grid on;
set(gca,'FontSize',20) %legend、坐标轴数字
set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);  %设置线宽
set(gcf,'Position',[500 350 640 360]); %前两个是初始位置坐标，后两个是初始分辨率