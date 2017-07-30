import cs50


# variable to store the pyramid size entered by the user
pyramidSize = -1

# ask user for a pyramid size between 0-23 inclusively
while ((pyramidSize < 0) or (pyramidSize > 23)):
    
    print("Height: ", end="")
    pyramidSize = cs50.get_int()
    
    
    
# counter to keep track of number of steps to print
steps = 1

# counter to keep track of number of spaces to print
spaces = pyramidSize - steps



# print out the pyramid    
for x in range(pyramidSize):
    
    # print the number of spaces needed
    print( " " * spaces, end="")
    
    # print the number of steps needed
    print( "#" * steps, end="")
    
    # print the 2 space gap
    print("  ", end="")
    
    # print the steps again with the newline this time
    print("#" * steps)
    
    # increment the number of steps by one, decrement spaces by one
    steps += 1
    spaces -= 1
    
    
    
    