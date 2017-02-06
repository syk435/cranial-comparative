function patient_Registration(preName, postName, corName, updatePostName)
    % Open pre-surgery patient mesh file. 
    fileID = fopen(preName);
    C_nx= textscan(fileID, '%d',1);
    n_x = C_nx{1}(1);
    X = zeros(n_x,3);
    for i = 1:n_x
        Cx = textscan(fileID,'%f %f %f',1);
        X(i,1)=Cx{1};
        X(i,2)=Cx{2};
        X(i,3)=Cx{3};
    end
    fclose(fileID);

    %Open post_surgery patient mesh file.
    fileID = fopen(postName);
    C_ny= textscan(fileID, '%d',1);
    n_y = C_ny{1}(1);
    Y = zeros(n_y,3);
    for i = 1:n_y
        Cy= textscan(fileID,'%f %f %f',1);
        Y(i,1)=Cy{1};
        Y(i,2)=Cy{2};
        Y(i,3)=Cy{3};
    end
    fclose(fileID);

    % Set the options
    opt.method='rigid'; % use rigid registration
    opt.viz=0;          % show every iteration
    opt.outliers=0.6;   % use 0.6 noise weight to add robustness 

    opt.normalize=1;    % normalize to unit variance and zero mean before registering (default)
    opt.scale=1;        % estimate global scaling too (default)
    opt.rot=1;          % estimate strictly rotational matrix (default)
    opt.corresp=1;      % do not compute the correspondence vector at the end of registration (default)

    opt.max_it=200;     % max number of iterations
    opt.tol=1e-8;       % tolerance

    % registering Y to X
    [Transform, C]=cpd_register(X,Y,opt);

    %figure,cpd_plot_iter(X, Y); title('Before');

    % X(Correspondence,:) corresponds to Y
    %figure,cpd_plot_iter(X, Transform.Y);  title('After registering Y to X');

    outID = fopen(corName,'w');
    fprintf(outID,'%d\r\n',C);
    fclose(outID);

    outID2 = fopen(updatePostName,'w');
    fprintf(outID,'%d \n',size(Transform.Y,1));
    for i = 1:size(Transform.Y,1)
        fprintf(outID2,'%f ',Y(i,:));
        fprintf(outID2,'\n');
    end
    fclose(outID2);
end

