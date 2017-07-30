import sys


def main():
    
    # ensure there are 2 arguments input
    if len(sys.argv) != 2:
        print("Correct usage: python greedy.py <int>")
        exit(1)
        
    # attempts to convert the key argument to an int.
    # if it's a char or anything non-int, exits with error.
    try:
        keyInt = int(sys.argv[1])
    except:
        print("Correct usage: python greedy.py <int>")
        exit(1)
    
    # ask user for text to cipher
    plaintext = input("plaintext: ")
    
    
    # print the ciphered text by calling cipherText function and exit
    # with 0
    print("ciphertext: {}".format(cipherText(plaintext, keyInt)))
    exit(0)
    



"""
    Function that, when passed in unciphered text and a key, will 
    cipher the text using Caesar's cipher. 
    
    Relies on cipherChar() to 
    change alphabetical characters into ciphered characters.
"""
def cipherText(unciphered, key):
    
    # remove unecessary extra in key. ie; 28 becomes 2, making a->c
    while key > 26:
        key -= 26
    
    # string to hold the soon-to-be ciphered text
    cipheredText = ""
    
    # itterate through each character in the unciphered text
    for char in unciphered:
        
        # if the character is not alphabetical, add it into the
        # ciphered text
        if not char.isalpha():
            cipheredText += char
            
        # otherwise, convert the character into ciphered text
        else:
            cipheredText += cipherChar(char, key)
    
    #return the now ciphered text
    return cipheredText




"""
    Function that, when passed in an aplabetial character and a key, 
    will use ceaser cipher to change the character depending on
    the key passed in. 
    
    Used in cipherText.
"""
def cipherChar(char, key):
    
    # check if the character is uppercase and, if it is...
    if char.isupper():
        
        # if the character + key would go beyond "Z", retract 26 places
        # to keep the character within the ascii capital letters and
        # return it.
        if ord(char) + key > ord("Z"):
            return chr((ord(char) + key) - 26)
            
        # otherwise, just add the key and return the new character.
        else:
            return chr(ord(char) + key)
            
    # otherwise, we do the above but for lowercase characters.
    else:
        if ord(char) + key > ord("z"):
            return chr((ord(char) + key) - 26)
        else:
            return chr(ord(char) + key)





if __name__ == "__main__":
    main()