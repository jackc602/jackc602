"""
CS190
Jack Carroll and Michael Maaseide
Final Project
Fall 2023
"""

FILENAME = "EPA_SmartLocationDatabase_V3_Jan_2021_Final.csv"
import csv
import matplotlib.pyplot as plt

def read_csv(filename):
    """
    Parameters: name of a csv file
    Returns: A 2D list
    Does: Takes dataset and converts it into a 2D list where interior lists
    are the contents of a single row
    """
    data = []
    with open(filename, "r") as infile:
        csv_file = csv.reader(infile)
        next(csv_file)
        for line in csv_file:
            data.append(line)
            
    return data


def get_column(lst, col_idx, city, city_col_idx):
    """
    Parameters: a 2d list, an index (int), and city name (str)
    Returns: a list
    Does: finds a singular statistic and stores it in a list for the rows that
    contain the city name parameter
    """
    # Initialize list
    statistic = []
    # Iterate through 2D list
    for i in range(len(lst)):
        # Check if the city givin is in the column index of each element of 2d
        if city in lst[i][city_col_idx]:
            if lst[i][col_idx] != 0:
                # Append to list if it is not equal to 0
                statistic.append(lst[i][col_idx])
    
    return statistic


def create_scatter(xaxis, yaxis, sizes, cities, title, xlabel, ylabel):
    """
    Parameters: a list of ints for x axis, a list of ints for y axis, 
            a list of ints for bubble size, a list of cities to label each point, 
            string for graph title, string for x label, string for y label
    Returns: nothing
    Does: Creates a plot of desired data
    """
    # Loop to plot data from lists with shape o, and scale the size based on 
    # the population divided by a constant
    for i in range(len(xaxis)):
        plt.plot(xaxis[i], yaxis[i], "o", markersize = sizes[i] / 1000000, 
                 label = cities[i])
    
    # Adding title and labels
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    # Create legend with custom location
    plt.legend(bbox_to_anchor = (1.85, 1), loc = "upper right")


def convert_to_floats(lst):
    """
    Parameters: a list of strings
    Returns: a list of floats
    Does: converts list of strings to list of floats
    """
    lst = [float(lst[i]) for i in range(len(lst))]
    
    return lst


def find_avg(lst):
    """
    Parameters: a list of ints
    Returns: a float
    Does: calculates the average value of a list
    """
    avg = sum(lst) / len(lst)
    
    return avg


def find_std_dev(lst):
    """
    Parameters: a list of ints
    Returns: an int
    Does: calculates the standard deviation of a list
    """
    sum_lst = sum(lst)
    avg_lst = sum_lst / len(lst)
    
    numerator = 0
    
    for num in lst:
        term_raw = num - avg_lst
        term_clean = term_raw * term_raw
        numerator += term_clean
    
    inside_root = numerator / len(lst)
    std_dev = inside_root ** .5
    
    return std_dev


def find_cor_co(lstx, lsty, sdx, sdy):
    """
    Parameters: two lists of ints and the standard deviations of those lists
    Returns: an int
    Does: calculates the correlation coefficient of those lists
    """
    
    # Getting average of lstx
    avg_lstx = find_avg(lstx)
    
    # Getting average of lsty
    avg_lsty = find_avg(lsty)
    
    # Creating a list of terms in numerator of covariance for each list
    numeratorcovx = []
    numeratorcovy = []
    
    # Using for loop to get terms in numerator of covariance for lstx
    for num in lstx:
        term = num - avg_lstx
        numeratorcovx.append(term)
    
    # Using for loop to get terms in numerator of covariance for lsty
    for num in lsty:
        term = num - avg_lsty
        numeratorcovy.append(term)
    
    overall_numerator = 0
    
    # Getting numerator for the correlation coefficient
    for i in range(len(lstx)):
        term = numeratorcovx[i] * numeratorcovy[i]
        overall_numerator += term
    
    covariance = overall_numerator / len(lstx)
    
    cc = covariance / (sdx * sdy)
    
    correlation_coeficient = round(cc, 3)
        
    return correlation_coeficient


def main():
    
    # Calculating statistics for metropolitan areas
    # Put in metropolitan areas we want to analyze
    cities = ["Dallas-Fort Worth, TX-OK", "Phoenix-Mesa, AZ", 
              "Los Angeles-Long Beach, CA", 
              "Boston-Worcester-Providence, MA-RI-NH-CT",
              "Kansas City-Overland Park-Kansas City, MO-KS",
              "New York-Newark, NY-NJ-CT-PA", 
              "Milwaukee-Racine-Waukesha, WI", "Charlotte-Concord, NC-SC",
              "Cleveland-Akron-Canton, OH", 
              "Orlando-Lakeland-Deltona, FL"]
    
    # Read in all data into a 2D list
    data = read_csv(FILENAME)
    
    # Creating a list of the column indexes of statistics we need to access
    # Indexes in order: walkscore, pct lowwage, pct working age, 
    # pct owning 0 cars
    stat_idx = [114, 32, 21, 23, 53]
    # Create 2D list with empty lists so we can append to each index later
    statistics_m = []
    for i in range(len(stat_idx)):
        statistics_m.append([])
    
    # Generate average statistics and add them to our 2D list 
    # Iterate for each index we defined
    for i in range(len(statistics_m)):
        # Then iterate over the list of cities
        for element in cities:
            # Get the column of data for the city of current iteration
            data_str = get_column(data, stat_idx[i], element, 8)
            # Convert column to floats
            data_flt = convert_to_floats(data_str)
            # Append the average to a list in our 2D list
            statistics_m[i].append(find_avg(data_flt))
    
    # Create list with populations of metro areas
    city_populations_m = []
    for i in range(len(cities)):
        # Get the column of population of each geographical subdvision of 
        # the metro area we are looking at 
        pops_str = get_column(data, 18, cities[i], 8)
        # Convert column to floats
        pops_flt = convert_to_floats(pops_str) 
        # Sum the population of each subdivision of the metro area we are 
        # looking at to get the total population of that area
        city_populations_m.append(sum(pops_flt))
        
    # Create list with occupied households per person 
    households_per_person_m = []
    for i in range(len(cities)):
        # Get the columnn of occupied households of each geographical 
        # subdivision in the metro area we are looking at 
        hhpp_str = get_column(data, 19, cities[i], 8)
        # Convert column to floats
        hhpp_flt = convert_to_floats(hhpp_str)
        # Divide the sum of occupied households by the popuation of that area 
        # to get occupied househols per person
        households_per_person_m.append(sum(hhpp_flt) / city_populations_m[i])
   
    
    # Plotting statistics for metropolitan areas
    # Scatterplot of average perecnt of low wage workers
    create_scatter(statistics_m[0], statistics_m[1], city_populations_m,
                   cities,  
        "Walkscore vs. Percentage of Low Wage Workers in Metropolitan areas",
                    "Average Walkscore", "Percentage of Low Wage Workers")
    plt.xlim(5, 15)
    plt.ylim(0.15, 0.35)

    plt.show()
    
    # Scatterplot of average percent of working age people
    create_scatter(statistics_m[0], statistics_m[2], city_populations_m, 
                   cities, "Walkscore vs. Percent of Working Age People in \n"
                   "Metropolitan Areas", "Average Walkscore", 
                   "Percentage of Working age People")
    plt.xlim(5, 15)
    plt.ylim(0.55, 0.65)
    
    plt.show()
    
    # Scatterplot of average percent of households owning no cars
    create_scatter(statistics_m[0], statistics_m[3], city_populations_m, 
                    cities, "Walkscore vs. Percent of Households Not Owning\n"
                    "Cars in Metropolitan Areas", "Average Walkscore", 
                    "Percentage of Households Not Owning Cars")
    plt.xlim(5, 15)
    plt.ylim(0, 0.4)
    
    plt.show()
    
    # Scatterplot of occupied households per person
    create_scatter(statistics_m[0], households_per_person_m, city_populations_m,
                   cities, "Walkscore vs. Occupied Housing units per Person\n" 
                   "in Metropolitan Areas", "Average Walkscore", 
                   "Occupied Households per Person")
    
    plt.xlim(5, 15)
    plt.ylim(0.3, 0.5)
    
    # Scatterplot of average gross employement density
    plt.show()
    create_scatter(statistics_m[0], statistics_m[4], city_populations_m, 
                   cities, "Walkscore vs. Average Gross Employement Density\n"
                   "in Metropolitan Areas", "Average Walkscore", 
                   "Average Gross Employement Density")
    
    plt.xlim(5, 15)
    plt.ylim(0, 20)
   

    # Calculating statistics for cities
    # Get city area names that we want to use
    counties = ["Lincoln, NE", "Boston-Cambridge-Newton, MA-NH", 
                "Richmond, VA", "Huntsville, AL", 
                "Los Angeles-Long Beach-Anaheim, CA", 
                "Tampa-St. Petersburg-Clearwater, FL", 
                "Chicago-Naperville-Elgin, IL-IN-WI", "Baton Rouge, LA", 
                "New York-Newark-Jersey City, NY-NJ-PA", "Columbus, OH"]

    # Creat a 2D list with empty lists inside of it so we can append to each 
    # list inside later
    statistics_c = []
    for i in range(len(stat_idx)):
        statistics_c.append([])
    
    # Generate average statistics and add them to our 2D list 
    # Iterate for each index we defined
    for i in range(len(statistics_c)):
        # Then iterate over the list of counties
        for element in counties:
            # Get the column of data for the city of current iteration
            data_str = get_column(data, stat_idx[i], element, 10)
            # Convert the column to floats
            data_flt = convert_to_floats(data_str)
            # Append the average to a list in our 2D list
            statistics_c[i].append(find_avg(data_flt))
    
    # Create list with populations of cities
    city_populations_c = []
    for i in range(len(counties)):
        # Get the column of population of each geographical subdvision of 
        # the city we are looking at 
        pops_str = get_column(data, 18, counties[i], 10)
        # Convert column to floats
        pops_flt = convert_to_floats(pops_str)
        # Sum the population of each subdivision of the city we are looking at 
        # to get the total population of that area
        city_populations_c.append(sum(pops_flt)) 
        
    # Create list with occupied households per person 
    households_per_person_c = []
    for i in range(len(counties)):
        # Get the columnn of occupied households of each geographical 
        # subdivision in the city we are looking at 
        hhpp_str = get_column(data, 19, counties[i], 10)
        # Convert column to floats
        hhpp_flt = convert_to_floats(hhpp_str)
        # Divide the sum of occupied households by the popuation of that area 
        # to get occupied househols per person
        households_per_person_c.append(sum(hhpp_flt) / city_populations_c[i])
        
    
    # Plotting for cities
    # Scatterplot of average perecnt of low wage workers
    plt.show()
    create_scatter(statistics_c[0], statistics_c[1], city_populations_c,
                   counties,  
        "Walkscore vs. Percentage of Low Wage workers in Cities",
                    "Average Walkscore", "Percentage of Low Wage Workers")
    plt.xlim(5, 15)
    plt.ylim(0.15, 0.35)
    
    # Scatterplot of average percent of working age people
    plt.show()
    create_scatter(statistics_c[0], statistics_c[2], city_populations_c, 
                   counties, "Walkscore vs. Percent of Working Age People in \n"
                   "Cities", "Average Walkscore", 
                   "Percentage of Working age People")
    plt.xlim(5, 15)
    plt.ylim(0.55, 0.65)
    
    
    # Scatterplot of average percent of households owning no cars
    plt.show()
    create_scatter(statistics_c[0], statistics_c[3], city_populations_c, 
                    counties, "Walkscore vs. Percent of Households Not Owning\n"
                    "Cars in Cities", "Average Walkscore", 
                    "Percentage of Households Not Owning Cars")
    plt.xlim(5, 15)
    plt.ylim(0, 0.4)

    # Scatterplot of occupied households per person
    plt.show()
    create_scatter(statistics_m[0], households_per_person_c, 
                   city_populations_c, cities, 
                   "Walkscore vs. Occupied Housing units per Person in Cities", 
                   "Average Walkscore", "Occupied Households per Person")
    plt.xlim(5, 15)
    plt.ylim(0.3, 0.55)
    
    # Scatterplot of average gross employement density
    plt.show()
    
    create_scatter(statistics_c[0], statistics_c[4], city_populations_c, 
                   counties, "Walkscore vs. Average Gross Employement Density\n"
                   "in Cities", "Average Walkscore", 
                   "Average Gross Employement Density")
    
    plt.xlim(5, 15)
    plt.ylim(0, 22)
    
    
    
    # Calculating Correlation Coefficients - for metropolitan areas
    # Appending household occupancy stat to stat lists
    statistics_m.append(households_per_person_m)
    statistics_c.append(households_per_person_c)
    
    # Creating empty lists to use in for loops
    std_dev_m = []
    var_stat_m = statistics_m[1:]
    cor_co_m = []
    
    # Getting standard deviations of every one of our statistic lists
    for lst in statistics_m:
        dev = find_std_dev(lst)
        std_dev_m.append(dev)
        
    var_std_dev_m = std_dev_m[1:]
    
    # Getting correlation coefficients of statistic compared to walkability
    for i in range(len(var_stat_m)):
        c = find_cor_co(statistics_m[0], var_stat_m[i], std_dev_m[0],
                        var_std_dev_m[i])
        cor_co_m.append(c)
    
     
    # Calculating Correlation Coefficients - for cities
    # Creating empty lists to use in for loops
    std_dev_c = []
    var_stat_c = statistics_c[1:]
    cor_co_c = []
    
    # Getting standard deviations of every one of our statistic lists
    for lst in statistics_c:
        dev = find_std_dev(lst)
        std_dev_c.append(dev)
        
    var_std_dev_c = std_dev_c[1:]
    
    
    # Getting correlation coefficients of statistic compared to walkability
    for i in range(len(var_stat_c)):
        c = find_cor_co(statistics_c[0], var_stat_c[i], std_dev_c[0],
                        var_std_dev_c[i])
        cor_co_c.append(c)
    
    var_titles = ["Percentage of low wage workers", 
                  "Percentage of working age people", 
                  "Percentage of households not owning cars",
                  "Average gross employment density",
                  "Occupied housing units per person"]
    
    # Communicating the correlation coefficients by type of area
    print("Correlation coefficients of metropolitan area walkscores vs:\n",
          var_titles[0], ":", cor_co_m[0], "\n",
          var_titles[1], ":", cor_co_m[1], "\n",
          var_titles[2], ":", cor_co_m[2], "\n",
          var_titles[3], ":", cor_co_m[3], "\n",
          var_titles[4], ":", cor_co_m[4], "\n")
    
    print("Correlation coefficients of city walkscores vs:\n",
          var_titles[0], ":", cor_co_c[0], "\n",
          var_titles[1], ":", cor_co_c[1], "\n",
          var_titles[2], ":", cor_co_c[2], "\n",
          var_titles[3], ":", cor_co_c[3], "\n",
          var_titles[4], ":", cor_co_c[4], "\n", "\n")
    
    # Calculating if the correlation is greater than .5 or less than -.5
    # If yes then communicating that the correlation between walkability and
    # That variable is strong
    print("Correlation coefficients that are stong:")
    for i in range(len(cor_co_m)):
        if cor_co_m[i] > .5 :
            print("Walkbility and", var_titles[i], "for metropolitan areas")
    if cor_co_m[i] < -.5:
        print("Walkbility and", var_titles[i], "for metropolitan areas")
    
    for i in range(len(cor_co_c)):
        if cor_co_c[i] > .5 :
            print("Walkbility and", var_titles[i], "for cities")
        if cor_co_c[i] < -.5:
            print("Walkbility and", var_titles[i], "for cities")
    
main()
