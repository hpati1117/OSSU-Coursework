import cs50

# get from user amount owed
amountOwed = -1
while amountOwed < 0:
    print("How much is owed?: ", end="")
    amountOwed = cs50.get_float()

# initialize coins to keep track of the total number of coins needed
coins = 0

# while the amount owed doesn't go below 0, keep using coins to reach
# the minimum amount needed to make change for the amount
while amountOwed != 0.0:
    
    if amountOwed - .25 >= 0:
        coins += 1
        amountOwed -= .25
        
    elif amountOwed - .10 >= 0:
        coins += 1
        amountOwed -= .10
    
    elif amountOwed - .05 >= 0:
        coins += 1
        amountOwed -= .05
        
    elif amountOwed - .01 >= 0:
        coins += 1
        amountOwed -= .01
        
# print the number of coins used
print(coins)
        
    

