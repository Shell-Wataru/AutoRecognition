addpath('./FaceRecog');

%データのロード
%  aoi=importdata('aoi5.txt');
%  save aoi.mat aoi
%  kuro=importdata('kuro5.txt');
%  save kuro.mat kuro
%  liang=importdata('liang5.txt');
%  save liang.mat liang
%  shell=importdata('shell5.txt');
%  save shell.mat shell 
%  ushi=importdata('ushi5.txt');
%  save ushi.mat ushi

clear MEMBERS;
ORIG_MEMBERS = {'aoi','kuro','liang','shell','ushi'};
index = 1:5;
count=0;
for ii=index
    count = count + 1;
    MEMBERS{count} = ORIG_MEMBERS{ii};
end;

X = [];
for ii=1:length(MEMBERS)
  load(sprintf('%s.mat',MEMBERS{ii}));
  eval(sprintf('v{%d} = %s;',ii,MEMBERS{ii}));
  X = [X;v{ii}];
end;

%  
% % データの平均値からの偏差
%   P_a = mface(aoi,3);
%   P_k = mface(kuro,3);
%   P_l = mface(liang,3);
%   P_s = mface(shell,3);
%   P_u = mface(ushi,3);

%   v_a = aoi*P_a;
%   v_k = kuro*P_k;
%   v_l = liang*P_l;
%   v_s = shell*P_s;
%   v_u = ushi*P_u;
%  
% % %  3次元の場合
% %  hold on
% %  scatter3(v_a(:,1),v_a(:,2),v_a(:,3)); %赤
% %  scatter3(v_k(:,1),v_k(:,2),v_k(:,3)); %緑
% %  scatter3(v_l(:,1),v_l(:,2),v_l(:,3));     %青
% %  scatter3(v_s(:,1),v_s(:,2),v_s(:,3));   %シアン
% %  scatter3(v_u(:,1),v_u(:,2),v_u(:,3)); %マゼンタ
% %  grid on
% %  hold off
%  
[n z] = size(v{ii});
%  

Y = [];
for ii=1:length(MEMBERS)
    label{ii} = -ones(n,length(MEMBERS));
    label{ii}(:,ii) = ones(n,1);
    Y = [Y;label{ii}];
end;

%   label_1 = [ones(n,1),-ones(n,1),-ones(n,1),-ones(n,1),-ones(n,1)];
%   label_2 = [-ones(n,1),ones(n,1),-ones(n,1),-ones(n,1),-ones(n,1)];
%   label_3 = [-ones(n,1),-ones(n,1),ones(n,1),-ones(n,1),-ones(n,1)];
%   label_4 = [-ones(n,1),-ones(n,1),-ones(n,1),ones(n,1),-ones(n,1)];
%   label_5 = [-ones(n,1),-ones(n,1),-ones(n,1),-ones(n,1),ones(n,1)];
%  
% % lab_1 = [ones(n,1),-ones(n,1)];
% % lab_2 = [-ones(n,1),ones(n,1)];
%  
% data = [[v_a;v_k;v_l;v_s;v_u],[label_1;label_2;label_3;label_4;label_5]];
% % data2 = [[v_l;v_s],[lab_1;lab_2]];
%X = [v_a;v_k;v_l;v_s;v_u];
%X = round(X);
%X = abs(X);
%Y = [label_1;label_2;label_3;label_4;label_5];


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Taiji Suzuki added
%PCA
options.issym = 1;
[n dd] = size(X);
[V,D] = eigs(@(x)PCA_fun(x,X),dd,n,'lm',options);

meanX = mean(X,1);
X = X-ones(size(X,1),1)*meanX;

threath = 0.1;
for nn=n:-1:1
    XX = X*V(:,1:nn);
    error_ratio = norm(X-XX*V(:,1:nn)','fro')/norm(X,'fro');
    if error_ratio > threath
        break;
    end;
end;
nn = min(nn+1,n);
V = V(:,1:nn)*inv(sqrt(D(1:nn,1:nn)));
XX = X*V;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

dtr = data(XX,Y);
% save data4_2men.mat data
%  
% 
%考慮すべきパラメータ
%C for mc_svm, threth for PCA, できるだけ多くのトレーニングデータ
%[r,a]=train(mc_svm({kernel('rbf',2),'C=10'}),dtr);
[r,a]=train(mc_svm({kernel('linear'),'C=0.5'}),dtr);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Taiji Suzuki added
vv = a.alpha'*XX*V'; 
%[tmp i] = max(vv*(input_image - meanX));
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

base_dir = 'C:/Users/t2ladmin/Documents/MATLAB/facerecog';
%base_dir = 'C:/Users/Suzuki/Documents/Study/FaceRecognition/FaceRecog/FaceRecog';
work_str = sprintf('%s/learn_vv.txt',base_dir);
write_vv(vv,work_str);
%base_dir = 'C:/Users/Suzuki/Documents/Study/FaceRecognition/FaceRecog/FaceRecog';
%work_str = sprintf('%s/learn_b.txt',base_dir);
%write_vv(a.b0,work_str);
 
 %[r,a]=train(one_vs_rest(svm(kernel('rbf',2))),dtr);
 % Test class centers
 % dtest=data([v_a;v_k;v_l;v_s;v_u]);
 % rtest=test(a,dtest);
 % plot(a);
% 
% % x = data(:, 1:3);                      %学習データ（入力ベクトル）
% % y = data(:, 4:8);                       %正解データ（ターゲットベクトル）
% % データを表示
% %disp([x  y]);
% 
% % kernel = 'spline'; 
% % 
% % [nsv, alpha, b0] = svc(x, y, kernel) ;      % x : 入力ベクトル y : ターゲットベクトル
% %                                            %　詳細は >> help svc を参照
% % [h] = svcplot(x, y, kernel, alpha, b0)     %結果をプロット
% 
% 
% 


