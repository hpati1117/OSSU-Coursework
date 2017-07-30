from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from time import sleep

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")



@app.route("/")
@login_required
def index():
    
    # if the user somehow got to post, return an error
    if request.method == "POST":
        return apology("Error, got to post?")
        
    # otherwise, everything is working properly so..
    else:
        
        # get the user's id from the current session
        userID = session["user_id"]
        
        # use the id to find their username
        userName = db.execute("SELECT username FROM users WHERE id = :userid",\
                               userid=userID)
        
        # variable to store the user's table portfolio name 
        # (username_portfolio)
        userTableName = userName[0]["username"] + "_portfolio"
        
        # try to create a new table for user's portfolio if they don't
        # already have one
        try:
            db.execute("CREATE TABLE :tablename ('userid' INTEGER NOT NULL,\
                    'symbol' TEXT NOT NULL, 'shares' INTEGER NOT NULL,\
                    'price' NUMERIC NOT NULL, 'total' NUMERIC NOT NULL)",\
                    tablename=userTableName)
        # otherwise, pass this step and move on                
        except:
            pass

        # get the symbol and shares for each stock from the user's portfolio
        userPortfolio = db.execute("SELECT shares, symbol FROM :tablename",\
                                    tablename=userTableName)
        
        # dictionary to hold the stock's information 
        indexPortfolio = {}
        
        # variable to hold the user's grand total
        userGrandTotal = 0
        
        # for each row which consists of symbol and shares...
        for row in userPortfolio:
            
            # lookup the stock's name, price, and symbol
            stockInfo = lookup(row["symbol"])
            
            # store the stock's name, symbol, user's number of owned
            # shares, current price of the stock, and the value of
            # the stock in appropriate variables. Also stores the
            #current price of the stock and it's value as floats
            #to use in calculations
            stockName = stockInfo["name"]
            stockSymbol = stockInfo["symbol"]
            stockShares = row["shares"]
            floatCurPrice = stockInfo["price"]
            floatCurValue = stockShares * floatCurPrice
            stockCurPrice = usd(floatCurPrice)
            stockCurValue = usd(floatCurValue)
            
            # add the value of the stock to the user's grand total
            userGrandTotal += floatCurValue
            
            # append each value to the dictionary as a tuple
            indexPortfolio[stockName] = (stockName, stockSymbol, stockShares,\
                                         stockCurPrice, stockCurValue)
        
        # gets the user's current balance
        userCash = db.execute("SELECT cash FROM users WHERE id = :userid",\
                               userid=userID)
        
        # adds the user's current balance to the grand total
        userGrandTotal += userCash[0]["cash"]

        # renders out the html for index, creating a table from the 
        # information in indexPortfolio and displaying the user's
        # current balance and grand total.
        return render_template("index.html", Profile=indexPortfolio,\
                                Balance=usd(userCash[0]["cash"]),\
                                GrandTotal=usd(userGrandTotal))



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if the user filled out the form and hit submit...
    if request.method == "POST":
        
        # check if user left a field empty
        if request.form.get("symbol") == "" or \
           request.form.get("quantity") == "":
            return apology("No field can be left blank")
        
        # if the quantity entered is greater than or equal to 0, redirect
        # to apology
        elif int(request.form.get("quantity")) <= 0:
            return apology("Can't buy a negative or 0 amount")
            
        # if the symbol doesn't exist, redirect to apology
        elif lookup(request.form.get("symbol")) == None:
            return apology("No company with that symbol found")
        
        # otherwise, if all fields are correct, then...
        else:
            
            # set up variables for frequently used things
            symbol = request.form.get("symbol").upper()
            quantity = int(request.form.get("quantity"))
            userID = session["user_id"]
            stockInfo = lookup(symbol)
            userCash = db.execute("SELECT cash FROM users WHERE id = :userid",\
                        userid=userID)
            
            # if the user cant afford it, redirect to apology
            if float(userCash[0]["cash"]) < stockInfo["price"] * quantity:
                return apology("Not enough money")
                
            # add purchase to history
            db.execute("INSERT INTO history (userid, symbol, quantity, cost)\
                        VALUES (:userid, :symbol, :quantity, :cost)",\
                        userid=userID, symbol=symbol, quantity=quantity,\
                        cost= -(stockInfo["price"] * quantity))
                        
            # update users cash amount
            db.execute("UPDATE users SET cash = cash - :total WHERE id = :userid",\
                        total=stockInfo["price"] * quantity, userid=userID)
                        
            # variable to hold user's portfolio table name in db
            userName = db.execute("SELECT username FROM users WHERE id=:userid",\
                                   userid=userID)
            userPortfolio = userName[0]["username"] + "_portfolio"
          
            # try to select the user's shares for that symbol. if it doesn't
            # exist yet, insert it into the table with default values of 0.
            userShare = db.execute("SELECT shares FROM :tablename WHERE symbol = :symbol",\
                                    tablename=userPortfolio, symbol=symbol)
            if not userShare:
                db.execute("INSERT INTO :tablename (userid, symbol, shares, price, total)\
                            VALUES (:userid, :symbol, :shares, :price, :total)",\
                            tablename=userPortfolio, userid=userID, symbol=symbol,\
                            shares=0, price=0, total=0)
                userShare = db.execute("SELECT shares FROM :tablename WHERE symbol = :symbol",\
                                    tablename=userPortfolio, symbol=symbol)

            # increment the number of shares in user's portfolio
            db.execute("UPDATE :tablename SET shares=:shares, price=:price, total=:total\
                        WHERE symbol = :symbol", tablename=userPortfolio,\
                        shares=userShare[0]["shares"] + quantity, price=stockInfo["price"],\
                        total=stockInfo["price"] * (userShare[0]["shares"] + quantity),\
                        symbol=symbol)

            # upon success, redirects to index
            return redirect(url_for("index"))
    
    # otherwise, show the user the form      
    else:
        return render_template("buy.html")




@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    # get a list of dictionaries with each history purchase
    userHistory = db.execute("SELECT * FROM history WHERE userid = :userid",\
                              userid=session["user_id"])
    
    # list to hold tuples of data that will be passed into history.html 
    formattedHistory = []
     
    # itterate through each dictionary returned and get the symbol,
    # quantity, cost, and date of each transaction and insert a tuple of
    # this data into the list formattedHistory newest to oldest
    for eachDict in userHistory:
        stockSymbol = eachDict["symbol"]
        stockQuantity = eachDict["quantity"]
        stockCost = usd(eachDict["cost"])
        stockDate = eachDict["date"]
        
        formattedHistory.insert\
        (0, (stockSymbol, stockQuantity, stockCost, stockDate))
        
    # Show user their history
    return render_template("history.html", historyList=formattedHistory)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")



@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))



@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if the user is inputting a post request...
    if request.method == "POST":
        
        # variable to hld the stock symbol
        stockSymbol = request.form.get("symbol")
        
        # get stock's info and check if none. if so, redirect to apology
        stockInfo = lookup(stockSymbol)
        if stockInfo == None:
            return apology("Couldn't find that stock")
            
        # display stock info to user
        return quoted(stockInfo["name"], stockInfo["symbol"], stockInfo["price"])
        
    # otherwise, load the form
    else:
        return render_template("quote.html")



@app.route("/quoted")
def quoted(name, symbol, price):
    """Display stock quote."""
    return render_template("quoted.html", \
    stockName=name, stockSymbol=symbol, stockPrice=price)
    


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # if user is trying to register, then...
    if request.method == "POST":
        
        # if the user left a field blank, redirect to apology
        if request.form.get("username") == "" or\
           request.form.get("password") == "" or\
           request.form.get("retypepassword") == "":
            return apology("No fields can be left blank")
        
        # else if passwords don't match, redirect to apology
        elif request.form.get("password") != request.form.get("retypepassword"):
            return apology("Passwords don't match")
        
        # else, check if the username hasn't been taken and, if so, 
        # register the user and log them in.
        else:
            
            # store the hashed password into hashpass to later store 
            hashpass = pwd_context.hash(request.form.get("password"))
            
            # try to insert the new user but, if fail, tell user username
            # was taken
            result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashpass)",\
                     username=request.form.get("username"), hashpass=hashpass)
            if not hashpass:
                return apology("Username taken")
                
            # log the new user in
            # query database for username
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

            # ensure username exists and password is correct
            if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
                return apology("invalid username and/or password")

            # remember which user has logged in
            session["user_id"] = rows[0]["id"]

            # redirect user to home page
            return redirect(url_for("index"))
            
    # else if they came through a get request, show them the correct page    
    else:
        # display the proper webpage form    
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    # if the user filled out the form...
    if request.method == "POST":
        
        # store the filled out form queries into variables for
        # ease of use along with user's id, username, and portfolio name
        stockSymbol = request.form.get("symbol").upper()
        stocksToSell = int(request.form.get("quantity"))
        userID = session["user_id"]
        userName = db.execute("SELECT username FROM users WHERE id = :userid",\
        userid=userID)
        userPortfolioName = userName[0]["username"] + "_portfolio"
        
        # check that both fields are entered or return apology
        if stockSymbol == "" or stocksToSell == "":
            return apology("No field can be left blank")
        
        # look up the stock and check it exists
        stockInfo = lookup(stockSymbol)
        if stockInfo == None:
            return apology("Stock doesn't exist!")
            
        # find the amount the user owns of that stock and verify we got
        # a valid return value
        userQuantity = db.execute("SELECT shares FROM :tablename WHERE symbol = :symbol",\
                                   tablename=userPortfolioName, symbol=stockSymbol)
        if userQuantity == None:
            return apology("You don't own shares with " + stockSymbol)
            
        # verify the user can sell the amount of stocks they want to sell
        if userQuantity[0]["shares"] <= 0 or\
           userQuantity[0]["shares"] < stocksToSell:
            return apology("You can't sell that amount of stocks!")
        
        # update user's portfolio by deleting the row if they'll have no
        # more stocks with the company
        newShares = userQuantity[0]["shares"] - stocksToSell
        if newShares == 0:
            db.execute("DELETE FROM :tablename WHERE symbol = :symbol",\
            tablename=userPortfolioName, symbol=stockSymbol)
        else:
            db.execute("UPDATE :tablename SET shares = :shares,\
                        price = :price, total = :total\
                        WHERE symbol = :symbol", tablename=userPortfolioName,\
                        shares=newShares, price=stockInfo["price"],\
                        total=stockInfo["price"] * newShares, symbol=stockSymbol)
        
        # update the user's cash balance
        db.execute("UPDATE users SET cash = cash + :newcash WHERE id = :userid",\
                    newcash=stocksToSell * stockInfo["price"], userid=userID)
                    
        # add the sale to history
        db.execute("INSERT INTO history (userid, symbol, quantity, cost)\
                    VALUES (:userid, :symbol, :quantity, :cost)",\
                    userid=userID, symbol=stockSymbol,\
                    quantity= -stocksToSell,\
                    cost=stocksToSell * stockInfo["price"])
        
        # return the user to index
        return redirect(url_for("index"))
    
    # render sell.html if the user wants to sell stocks
    else:
        return render_template("sell.html")
    


@app.route("/add_funds", methods=["GET", "POST"])
@login_required
def add_funds():
    
    # if the user filled in the form...
    if request.method == "POST":
        
        # store the amount to add into a variable for ease of use
        amountToAdd = int(request.form.get("amount"))
        
        # add the funds to the user's cash value
        db.execute("UPDATE users SET cash = cash + :amount WHERE id = :userid",\
                    amount=amountToAdd, userid=session["user_id"])
        
        # redirect back to index
        return redirect(url_for("index"))
    
    # otherwise, show the user add_funds.html form
    else:
        return render_template("add_funds.html")
