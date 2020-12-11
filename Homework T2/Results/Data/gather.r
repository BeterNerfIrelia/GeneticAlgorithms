getq1=function(string)
{
  # %% 5 ==1
  vec = scan(string);
  len = length(vec);
  y=c();
  for(i in 1:len)
    if(i %% 5 == 1){
      y=c(y,vec[i]);
    }
  return(y);
}

getq2=function(string)
{
  # %% 5 ==2
  vec = scan(string);
  len = length(vec);
  y=c();
  for(i in 1:len)
    if(i %% 5 == 2){
      y=c(y,vec[i]);
    }
  return(y);
}

getq3=function(string)
{
  # %% 5 ==3
  vec = scan(string);
  len = length(vec);
  y=c();
  for(i in 1:len)
    if(i %% 5 == 3){
      y=c(y,vec[i]);
    }
  return(y);
}

getvalue=function(string)
{
  #  %% 5 == 4
  vec = scan(string);
  len = length(vec);
  y=c();
  for(i in 1:len)
    if(i %% 5 == 4){
      y=c(y,vec[i]);
    }
  return(y);
}

gettime=function(string)
{
  # %% 5 == 0
  vec = scan(string);
  len = length(vec);
  y=c();
  for(i in 1:len)
    if(i %% 5 == 0){
      y=c(y,vec[i]);
    }
  return(y);
}

get5=function(x)
{
  y=c();
  for(i in 1:30)
    y=c(y,x[i]);
  return(y);
}

get10=function(x)
{
  y=c();
  for(i in 31:60)
    y=c(y,x[i]);
  return(y);
}

get30=function(x)
{
  y=c();
  for(i in 61:90)
    y=c(y,x[i]);
  return(y);
}

getdata=function(x)
{
  mini=min(x);
  maxi=max(x);
  avg=mean(x);
  sigma=sd(x);
  y=c(mini,maxi,avg,sigma);
  return(y);
}

getall=function()
{
  names=c("DeJong","Rastrigin","Michalewicz","sqrt","atan");
  dimen=c(5,10,30);
  pop=c(100,200);
  gen=c(1000,10000);
  met=c("W","T","E");
  per=c(0,4,10);
  ksize=c(0,2,7);
  cross=c("E","R");
  path="files.txt";
  
  for(name in names)
  {
    for(popsize in pop)
    {
      for(gensize in gen)
      {
        final="final/";
        writePath=paste(final,name,met[1],".txt",sep="");
        #for(k in 2:3)
        {
          
        
          for(cx in cross)
          {
            stringname=paste(name,popsize,gensize,met[1],per[1],ksize[1],cx,".txt",sep="");
            
            q1=getq1(stringname);
            q2=getq2(stringname);
            q3=getq3(stringname);
            vec=getvalue(stringname);
            time=gettime(stringname);
            
            t5=get5(time);
            t10=get10(time);
            t30=get30(time);
            
            dt5=getdata(t5);
            dt10=getdata(t10);
            dt30=getdata(t30);
            datat=c(dt5,dt10,dt30);

            x5=get5(vec);
            x10=get10(vec);
            x30=get30(vec);
          
            q15=get5(q1);
            q110=get10(q1);
            q130=get30(q1);
          
            q25=get5(q2);
            q210=get10(q2);
            q230=get30(q2);
          
            q35=get5(q3);
            q310=get10(q3);
            q330=get30(q3);
          
            d5=getdata(x5);
            d10=getdata(x10);
            d30=getdata(x30);
          
            dq15=getdata(q15);
            dq110=getdata(q110);
            dq130=getdata(q130);
            dataq1=c(dq15,dq110,dq130);
          
            dq25=getdata(q25);
            dq210=getdata(q210);
            dq230=getdata(q230);
            dataq2=c(dq25,dq210,dq230);
          
            dq35=getdata(q35);
            dq310=getdata(q310);
            dq330=getdata(q330);
            dataq3=c(dq35,dq310,dq330);
          
            datav=c(d5,d10,d30);
          
            # WHEEL OF FORTUNE
          
            title = paste(popsize," ",gensize," ",met[1],cx,"   ",sep="");
            write(title,file=writePath,ncolumns = 2,append = TRUE);
            for(d in dimen)
            {
                ceva=paste(d);
              write(ceva,file=writePath,ncolumns=2,append=T);
              m=0;
              if(d==10)
                m=1;
              if(d==30)
                m=2;
              expr=paste(datav[1+4*m]," =Minimum");
              write(expr,file=writePath,ncolumns = 2,append = T);
            
              expr=paste(datav[2+4*m]," =Maximum");
              write(expr,file=writePath,ncolumns = 2,append = T);
            
              expr=paste(datav[3+4*m]," =Mean");
              write(expr,file=writePath,ncolumns = 2,append = T);
            
              expr=paste(datav[4+4*m]," =SD");
              write(expr,file=writePath,ncolumns = 2,append = T);
            
              ceva=paste("Q1");
              write(ceva,file=writePath,ncolumns = 2,append=T);
            
              expr=paste(dataq1[3+4*m]," =Q1 mean");
              write(expr,file=writePath,ncolumns = 2,append = T);
            
              ceva=paste("Q2");
              write(ceva,file=writePath,ncolumns = 2,append=T);
            
              expr=paste(dataq2[3+4*m]," =Q2 mean");
              write(expr,file=writePath,ncolumns = 2,append = T);
            
              ceva=paste("Q3");
              write(ceva,file=writePath,ncolumns = 2,append=T);
            
              expr=paste(dataq3[3+4*m]," =Q3 mean");
              write(expr,file=writePath,ncolumns = 2,append = T);
              
              ceva=paste("Time");
              write(ceva,file=writePath,ncolumns = 2,append=T);
              
              expr=paste(datat[3+4*m]," =Time mean");
              write(expr,file=writePath,ncolumns = 2,append = T);
            
            
              newline=paste("\n");
              write(newline,file=writePath,ncolumns=2,append = T);
            }
          
          
          }
          
          
          
          
        }
        
      }
    }
  }
}