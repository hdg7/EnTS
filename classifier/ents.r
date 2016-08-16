#!/usr/local/bin/Rscript

library(caret)
library("fpc")
library(ROCR)

args <- commandArgs(TRUE)

a <- read.table(args[1],sep=";",head=FALSE)
b <- as.matrix(a)
c <- read.table(args[4],head=FALSE)
rownames(b) <- t(c)

c <- read.table(args[3],head=FALSE)
c <- as.character(t(c))

correlations <- cor(b)
hcorrelations <- findCorrelation(correlations,cutoff=0.75)
save(hcorrelations,file=paste(args[2],"cor","RData",sep='.'))
space <- b[,-hcorrelations]
spaceC <- data.frame(c,space)
colnames(spaceC)[1] <- "Class"

#Selecting Random Index
benign <- 1:(trunc(nrow(spaceC)/2))
malware <- (trunc(nrow(spaceC)/2)+1):nrow(spaceC)

benignTrain <- benign[1:(trunc(length(benign)/3*2)+1)]
malwareTrain <- malware[1:(trunc(length(malware)/3*2)+1)]

benignTest <- benign[-match(benignTrain,benign)]
malwareTest <- malware[-match(malwareTrain,malware)]

trainindex <- c(benignTrain,malwareTrain)
testindex <- c(benignTest,malwareTest)

trainindex <- sample(trainindex)
testindex <- sample(testindex)

trainset <- spaceC[trainindex,]
testset <- spaceC[testindex,]

# create model using Forest
library(ipred)
library(rpart)
library(party)
library(e1071)
library(caret)

model <- cforest(as.factor(Class)~.,data=trainset,control = cforest_unbiased(ntree = 100))
prediction <- predict(model,  newdata=testset[,-1])
probabilities <- 1- unlist(treeresponse(model, testset), use.names=F)[seq(1,nrow(testset)*2,2)]
tab <- table(pred = prediction,true = testset[,1])
mconf <- confusionMatrix(tab)
print("Random Forest")
#print(tab)
print(mconf$overall[1])
rocr <- prediction(probabilities,testset[,1])
performance <- performance(rocr,"tpr","fpr")
pem<-cbind(as.matrix(slot(performance,"x.values")[[1]]),as.matrix(slot(performance,"y.values")[[1]]),as.matrix(slot(performance,"alpha.values")[[1]]))
write.table(file=paste(args[2],".rf.roc",sep=""),pem,col.names=FALSE,row.names=FALSE,sep=";")
save(model,file = paste(args[2],"RData",sep='.'))
namesba <- rownames(spaceC[testindex,])
classes <- cbind(namesba,as.character(prediction))
write.table(file=paste(args[2],".rf.class",sep=""),classes,sep=";",row.names=FALSE)

