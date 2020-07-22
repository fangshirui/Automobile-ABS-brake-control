%先导入中附着条件制动的数据



marker_idx = 1:50:400;
subplot(1,2,1)
plot(slip,'b-')

title('')
grid on;
axis([0 9 0 1]);%限制轴的显示宽度 前面是x轴，后面是y 轴
%%%%%%%%%%%基本设置%%%%%%%%%%%%%%
% 注意在matlab脚本语言使用的是GBK编码的格式
xlabel("Time","fontsize",12);  %标签文字大小
ylabel("Slip rate","fontsize",12);
%legend('High Adhesion coefficient','Middle Adhesion coefficient','Low Adhesion coefficient')
set(gca,'FontSize',10); %legend、坐标轴数字
set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);  %设置线宽
set(gcf,'Position',[500 100 640 360]); %前两个是初始位置坐标，后两个是初始分辨率
%%%%%%%%%%%基本设置%%%%%%%%%



subplot(1,2,2)
plot(Vehicle_speed,'bo-','MarkerIndices',marker_idx)
hold on;
plot(Wheel_speed,'r^-','MarkerIndices',marker_idx)
axis([0 9 0 30]);%限制轴的显示宽度 前面是x轴，后面是y 轴
grid on;



%%%%%%%%%%%基本设置%%%%%%%%%%%%%%
% 注意在matlab脚本语言使用的是GBK编码的格式
xlabel("Time","fontsize",12);  %标签文字大小
ylabel("Speed","fontsize",12);
legend('Vehicle speed','Wheel speed')

set(gca,'FontSize',10); %legend、坐标轴数字
set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);  %设置线宽
set(gcf,'Position',[500 100 720 480]); %前两个是初始位置坐标，后两个是初始分辨率

%%%%%%%%%%%基本设置%%%%%%%%%

% 绘制总标题
suptitle('Braking simulation under medium adhesion condition');





