#!/usr/local/bin/Rscript

#Arguments
# args[1] matrix
# args[2] image name
# args[3] image title
library(png)
args <- commandArgs(TRUE)

a <- read.table(args[1],sep=";",head=FALSE)
b <- as.matrix(a)
#b <- b[c(1:1000,2001:3000),c(1:1000,2001:3000)]
#pdf(args[4])
pdf(args[2])
x <- seq(1,length(b[1,]))
y <- seq(1,length(b[,1]))
image(x,y,b,col=paste("gray",99:1,sep=""))
title(main=args[3],cex.main=3)
dev.off()
