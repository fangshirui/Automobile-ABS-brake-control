%�ȵ����и��������ƶ�������



marker_idx = 1:50:400;
subplot(1,2,1)
plot(slip,'b-')

title('')
grid on;
axis([0 9 0 1]);%���������ʾ��� ǰ����x�ᣬ������y ��
%%%%%%%%%%%��������%%%%%%%%%%%%%%
% ע����matlab�ű�����ʹ�õ���GBK����ĸ�ʽ
xlabel("Time","fontsize",12);  %��ǩ���ִ�С
ylabel("Slip rate","fontsize",12);
%legend('High Adhesion coefficient','Middle Adhesion coefficient','Low Adhesion coefficient')
set(gca,'FontSize',10); %legend������������
set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);  %�����߿�
set(gcf,'Position',[500 100 640 360]); %ǰ�����ǳ�ʼλ�����꣬�������ǳ�ʼ�ֱ���
%%%%%%%%%%%��������%%%%%%%%%



subplot(1,2,2)
plot(Vehicle_speed,'bo-','MarkerIndices',marker_idx)
hold on;
plot(Wheel_speed,'r^-','MarkerIndices',marker_idx)
axis([0 9 0 30]);%���������ʾ��� ǰ����x�ᣬ������y ��
grid on;



%%%%%%%%%%%��������%%%%%%%%%%%%%%
% ע����matlab�ű�����ʹ�õ���GBK����ĸ�ʽ
xlabel("Time","fontsize",12);  %��ǩ���ִ�С
ylabel("Speed","fontsize",12);
legend('Vehicle speed','Wheel speed')

set(gca,'FontSize',10); %legend������������
set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);  %�����߿�
set(gcf,'Position',[500 100 720 480]); %ǰ�����ǳ�ʼλ�����꣬�������ǳ�ʼ�ֱ���

%%%%%%%%%%%��������%%%%%%%%%

% �����ܱ���
suptitle('Braking simulation under medium adhesion condition');





