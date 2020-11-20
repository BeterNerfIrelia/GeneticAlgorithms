getMinim=function(x)
{
  len=length(x);
  y=c();
  for(i in 1:len){
    if(i%%2==1){
      y=c(y,x[i]);
    }
  }
  return(y);
}

getTimes=function(x)
{
  len=length(x);
  y=c();
  for(i in 1:len){
    if(i%%2==0){
      y=c(y,x[i]);
    }
  }
  return(y);
}

getData=function(string)
{
  #methods=c("hc_best","hc_first","sa");
  methods=c("sa");
  dimensions=c(5,10,30);
  for(methodName in methods)
  {
    for(dim in dimensions)
    {
      path="Data\\";
      path=paste(path,string,dim,"_",methodName,"_data.txt",sep="");
      readPath=paste(string,dim,"_",methodName,".txt",sep = "");
      
      vec=scan(readPath);
      
      minima=getMinim(vec);
      times=getTimes(vec);
      
      minValue=min(minima);
      maxValue=max(minima);
      avgValue=mean(minima);
      sdValue=sd(minima);
      
      minTime=min(times);
      maxTime=max(times);
      avgTime=mean(times);
      sdTime=sd(times);
      
      stringValue=paste("Minimum value =",minValue);
      write(stringValue,file=path,ncolumns = 2,append = TRUE);
      
      stringValue=paste("Maximum value=",maxValue);
      write(stringValue,file=path,ncolumns = 2,append = TRUE);
      
      stringValue=paste("Avg Value =",avgValue);
      write(stringValue,file=path,ncolumns = 2,append = TRUE);
      
      stringValue=paste("SD value =",sdValue);
      write(stringValue,file=path,ncolumns = 2,append = TRUE);
      
      stringValue=paste("Minimum time =",minTime);
      write(stringValue,file=path,ncolumns = 2,append = TRUE);
      
      stringValue=paste("Maximum time =",maxTime);
      write(stringValue,file=path,ncolumns = 2,append = TRUE);
      
      stringValue=paste("AVG time =",avgTime);
      write(stringValue,file=path,ncolumns = 2,append = TRUE);
      
      stringValue=paste("SD time =",sdTime);
      write(stringValue,file=path,ncolumns = 2,append = TRUE);
      
    }
  }
}

getData("dejong")
getData("schwefel")
getData("rastrigin")
getData("michalewicz")