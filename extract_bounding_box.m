


function extract_bounding_box()
extractvbbfile('./data');
return    

function extractvbbfile(foldername)
name=sprintf('%s/seq/*.vbb',foldername);
list1=dir(name);
s=1;
e=length(list1);
for i=s:e
    filename = list1(i).name;
    %seperate the ext and name.
    [pathstr,name,ext] = fileparts(filename);
    fprintf(1,'extract the ts from %s\n',filename);
    video_name=sprintf('%s/seq/%s',foldername,name);
    vbb_name=sprintf('%s.vbb',video_name);
    seqname=sprintf('%s.seq',video_name);
    A=vbb('vbbLoad',vbb_name);
    nFrame=A.nFrame;
    sr=seqIo(seqname,'reader');
      sr.next();
    [stats,stats1,logDur] = vbb( 'getStats', A );
    fprintf(1,'extracting bounding box from %s\n',filename);
    for j=1:nFrame
        [gt,posv,lbls]=vbb('frameAnn',A,j,stats.labels);       
        newName = sprintf([name,'%05d.jpg'],j);
        [I,ts]=sr.getframe();
        if(~isempty(gt)) 
           len=size(gt,1);
           for m = 1:len   
            outfolder = sprintf('%s\\Result\\%s',foldername,name);
            if (~exist(outfolder,'dir'))    
                mkdir(outfolder);
            end
            outname=sprintf('%s/%s_%05d_%02d.jpg',outfolder,char(lbls{m}),j,m);
           
            x0=round(gt(m,2));
            x1=x0+round(gt(m,4));
            [x,y,z]=size(I);
            if (x1>x)
                x1=x;
            end
            y0=round(gt(m,1));
            y1=y0+round(gt(m,3));
            if (y1>y)
                y1=y;   
            end
            %                image=I(gt(k,2):(gt(k,2)+gt(k,4)),gt(k,1):(gt(k,1)+gt(k,3)),:);
            image=I(x0:x1,y0:y1,:);           
            imwrite(image, outname, 'Quality', 100); 
           end       
        end
        sr.next();
    end   
    fprintf(1,'done extracting bounding box from %s\n',filename);
end
return


    
