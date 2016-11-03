# EnTS

The Entropy Time Series algorithm aims to extract an entropy profile of a specific binary in order to generate a search space where a classifier can distinguish between malware and benign-ware using only the profile information. 

This repository is divided into two folder: 

- EntCalculator: Creates the Entropy Profiles and also contains a Structural Entropy implementation.
- Classifier: Classifies the profiles in different classes.

##Creating Entropy profiles

To create an entropy profile you need a configuration file, you can find an example in config.ini.

The command to create an Entropy Profile from a specific file is:
```
./entCalculator -i file.bin 
```
The output, depending on your configuration file, will provide you a lot of information about the original profile, the sub-profile, the wavelet and the final reconstruction. In EnTS we use the last one. If you want to create the space from a big set of files, you only need to put everything in a matrix style. I recommend you to use AWK for that:
```
awk 'FNR==1{print $1}' reconSeg* > ents.matrix
```

In the current format, ents.matrix must be class-balanced and it will be divided in 2/3 for train and 1/3 for test. Train and Test must be sorted too, in order to ensure that your test data is not contaminating your train set.

##Classification

Once you have the space I recommend you to set up the names and the classes in different files. Suppose we have all.names and all.classes with the names and the classes for malware and benign-ware. Then, we call the the classifier as follows:
```
./ents.r ents.matrix ENTS all.classes all.names
```
This generates an output and two files: ENTS.rf.roc and ENTS.rf.class. These files contains the roc curve and the individual classification of the test set.

##Structural Entropy

As you might notice there are some extra files and options inherit from Structural 
Entropy, you can generate the structural entropy segments with the -i option and 
once you have the segments, you can use EntCalculatos to calculate a pairwise 
comparison of all of them following structural entropy metric. To do this create a 
folder with the segment* files and run:
```
./entCalculator -d folder/
```
This command will print the similarity matrix.

##Visualization

We provide a small Shiny visualization framework for R. This is designed to show the original entropy profile, the 
chosen subsequence, the wavelet decomposition and the final reconstruction. EnTS uses the whole reconstraction, while
Structural Entropy uses only the average information from the selected segments (highlight in red). You can run it in
R (from the visualization folder) as follows:
```
>library(shiny)
>runApp(".")
```
In server.R you need to set the variable inEnTS to your entCalculator program

![alt tag](http://freedevelop.org/wp-content/uploads/DABE.png)
