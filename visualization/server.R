library(shiny)
inEnTS <- "../EntCalculator/entCalculator"

# Define server logic required to draw a histogram
shinyServer(function(input, output) {

  # Expression that generates a histogram. The expression is
  # wrapped in a call to renderPlot to indicate that:
  #
  #  1) It is "reactive" and therefore should re-execute automatically
  #     when inputs change
  #  2) Its output type is a plot
    
    output$distPlot <- renderPlot({
    if(input$tabs == "Profiles")
      {

        inFile <- input$file1
            if(is.null(inFile))
                return(NULL)
	    system("rm *temp");
	    system(paste("cp",inFile$datapath,"./temp",sep=" "));
        #print(inEnTS)
        #	    system(paste(inEnTS," -i temp",sep=" "));
        system(paste(inEnTS," -i temp",sep=" "));
	    a <- read.table(paste("orSeg","temp",sep=""),sep=";",header=FALSE);
	    b <- read.table(paste("subSeg","temp",sep=""),sep=";",header=FALSE);
	    c <- read.table(paste("wavelet","temp",sep=""),sep=";",header=FALSE);
	    d <- read.table(paste("reconSeg","temp",sep=""),sep=";",header=FALSE);
	    seg <- read.table(paste("segments","temp",sep=""),sep=";",header=FALSE);
	
	    par(mfrow=c(2,2))
	    plot(t(as.matrix(a)),type="l",main="Original",cex.main=3)
	    plot(t(as.matrix(b)),type="l",main="Subsequence",cex.main=3)
	    abline(v=as.integer(as.matrix(seg)),col="red")
	    plot(t(as.matrix(c)),type="l",main="Wavelet",cex.main=3)
	    plot(t(as.matrix(d)),type="l",main="Reconstruction",cex.main=3)
	    abline(v=as.integer(as.matrix(seg)),col="red")
      }
  })

})

