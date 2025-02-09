# Mortgage Default Predictor

## Project Overview

This project is a supervised learning task which aims to accurately predict whether mortgages will default based on features of the person taking out the loan and of the property itself. I built a feedforward neural network as my first experiment with NNs as well as a random forest as a means of comparison. The two largest challenges of this project were dealing with high percentages of missing values in many columns, as well as a highly imbalanced dataset by prediction class. After I did some exploration of the dataset, to deal with the missing values I dropped columns with over 60 percent of their values missing. After this I performed mean, median, and mode imputation depending on the nature of each column leaving no more missing data. To handle the imbalance, I used oversampling through SMOTE to create synthetic data points that left the classes with equal totals. At this point the data was ready for encoding, normalization, and splitting into train, test, and validation portions. 

## Results

I chose accuracy as the success metric to monitor because after the oversampling, the data was balanced thus accuracy couldn't be biased by over predicting a majority class. The neural network performed with a mean cross validated accuracy of **0.955** and an accuracy score of **0.956** on the validation portion of the data. The random forest model constructed after performed with a mean cross validated accuracy of **0.956**. Both models performed well across all splits of the data, however due to the early stoping methods used while training the NN, its training took significantly less time than the RF (about 1/2 of the time on my computer). Both models are viable methods for this task, with the NN being slightly less computationally intensive to train. 

View the notebook [here](https://github.com/jackc602/jackc602/blob/main/Projects/Mortgage%20Default%20Predictor/.ipynb/mortgage_analysis.ipynb).
