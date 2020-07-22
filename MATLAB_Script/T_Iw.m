clear 
clc 
close all

%初始化参数

I = 0:0.1:2;
w = 0:5:150;
r1=0.037;
r2=0.105; h=1*10^-3; n=3; niandu=0.24;
% dz=[127.1339 -318.0875 202.0143 40.8614 -0.4536]
[I,w] = meshgrid(I,w);

% 计算过程
xiaolv= 0.8;   % 修改效率值，改变B_I的关系
b=0.5146*I*xiaolv;  % 0.5146是b-I 的关系
tau = 127.1339*b.^4-318.0875*b.^3+202.0143*b.^2+40.8614*b-0.4536;
T=4/3*pi*n*tau*1000*(r2^3-r1^3)+pi*n*niandu*w/h*(r2^4-r1^4)+4;


% 绘图

mesh(I,w,T);


%标签设置

xlabel("I(A)","fontsize",20)  %标签文字大小
ylabel("\omega(rad/s)","fontsize",20)
zlabel("T(N/m)","fontsize",20)
%title("")


%图形的一些设置
%axis tight;
%colormap(get);
%shading interp;
set(gca,'FontSize',20) %legend、坐标轴数字
set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);  %设置线宽
set(gcf,'Position',[500 350 640 360]); %前两个是初始位置坐标，后两个是初始分辨率