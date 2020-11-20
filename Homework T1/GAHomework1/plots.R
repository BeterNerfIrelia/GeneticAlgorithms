drawingTimePlots=function()
{
  hcbest=c(25.752,152.425,404.555);
  hcfirst=c(13.738,77.707,1231.297);
  sa=c(25.605,36.531,61.965);
  
  plot(x=as.factor(c(5,10,30)),hcbest,xlim=c(1,3),ylim=c(0,5100),type="o",lty="blank",col="white",xlab="Dimensions",ylab="Time elapsed",main="De Jong's function");
  
  legend(1,5100,lty = "solid",lwd=3,legend = c("HC (Best)","HC (First)","SA"),col = c("black","green","brown"));
  
  lines(hcbest,type="o",lwd=3,col="black");
  lines(hcfirst,type="o",lwd=3,col="green");
  lines(sa,type="o",lwd=3,col="brown");
}

drawingTimePlots()