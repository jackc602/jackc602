# Modelling MLB Team Success

### Overview 
This is an early project performed collaboratively and aimed to predict what degree of playoff and regular season
success a professional baseball team will have. The methods we used were to create a linear regression model to predict 
amount of regular season wins, and a KNN classifier to predict labels depicting post season success. Both of these 
models took in the features we found to have the greatest corrrelation to team wins, consisting of team stats like hits, 
RBIs, and ERA, and saves across an entire season. 

### Regression Model

Based on these stats, we were able to predict a team's wins for a 
season with a mean absolute error of **3.07**, with some predictions visualized below. Note that we trained the model on data up to 2014, and tested it on statistics from the 2015 season. 

![Figure_2_good](https://github.com/user-attachments/assets/0e262edb-1b35-4770-b53f-86475779d838)

The predictions seemed to miss in a sort of normal distribution as pictured below, not favoring a certain type of error. 

![Figure_1_good](https://github.com/user-attachments/assets/bc2f13b1-ae13-4d5b-b919-f91780d56a1d)

Overall this model could be applied to early extrapolations of regular season statistics to get an idea of how well teams will do over the course of a season. 

### KNN Classifier

Our KNN classifier was largely ineffective, despite having an accuracy score of 94%. The reason for this was an abundant majority class for our predictions. Our labelling system was based on post-season success, which for a vast majority of teams is none, which we labelled accordingly. This is visualized when both of the plots below are viewed together. The same points that have a label other than 0 are incorrectly labelled by the model. 

![Figure_12_good](https://github.com/user-attachments/assets/c8a6e38b-96d5-4b3a-99fa-0644a0c2dd29)
![Figure_13_good](https://github.com/user-attachments/assets/4d877907-5875-4f62-80a0-e30c7ec8a30f)

This was an important early learning experience for me, the first takeaway being that accuracy isn't everything, and can be biased. For a classification task like this, a metric like precision or recall, or F1 combining them both would be an important thing to evaluate alongside accuracy to get the full picture. The second takeaway is that an early consideration of a classification task should be class balance. This could have been addressed using an over/undersampling technique in the early stages of the project. Overall this was a very informative and enjoyable project, I had a great time taking a deeper look into the statistics side of a aport I love. It's fun and instructive to look back and see what I could have done differently to glean better results.

View the code [here](https://github.com/jackc602/jackc602/blob/main/Projects/MLB%20Team%20Success/mlb_success_project.py).

