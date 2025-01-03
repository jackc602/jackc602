# Bizparse - An NLP Library for Business Documents

### Overview

This project aimed to take in business related documents, the examples used are all 10Ks, and provide 
multiple insightful visualizations that can help someone understand large scale trends in these documents.
This is not intended as a replacement for the understanding gained by carefully reading and thinking about 
the content of these documents, rather it's purpose is to one, provide insight quicker than reading an 
entire document and two, demonstrate large scale trends that aren't readily available to the human eye. 
This posed a creative challenge, to try and come up with insights into some incredibly scrutinized 
documents, however I believe myself and the three other peers of mine that worked on this came up with 
interesting visualizations that provide value to people with any level of experience with business lingo.

### Figures

Starting off we have a comparative tool, which works by measuring the individual counts of all words in 
two documents, then subtracting one of the wordcounts from the other and plotting the most common words
plus vice versa. This shows you the main difference in what is being discussed in two different documents.
There were several interesting results of this tool being used in different cases. The example below
showcases the difference between Nvidia and AMD, two competing companies who have had drastically different
performances in the recent rise of AI despite the fact that they both manufacture graphics cards. The 
things being discussed in their respective 10K filings could be indicators of why this happened. Nvidia 
places more emphasis on the words impact, demand, and networking while AMD contained far more occurences of 
agreement, company, broadcom, and technology. Nvidia seemed to be focused on larger scale external topics 
and less focused on the internal workings of their company.

![better_comp_bar](https://github.com/user-attachments/assets/feff4a94-9826-4bcf-b451-0fd55a6619b3)

Next we created a bar chart which analyzed the sentiment score of sentences conatining different words. 
We created a custom text parser which split the text into individual sentences, then search for the 
desired keywords and when they were found calculated a sentiment score ranging from -1 to 1 of that sentence
then averaged the score of all sentences where that specific word occurred. In the example below, again of
Nvidia and AMD, there are significant differences in the sentiments surrounding cost, competitor, and 
profit. What's particulary interesting is that AMD talks much more positively about profit, when it has 
certainly had a much less profitable year that Nvidia.

![Figure_1](https://github.com/user-attachments/assets/7897b739-e531-4f46-b164-3ab0064f41fd)



