from flask import Flask, redirect, render_template, request, url_for

import os
import sys

import helpers
from analyzer import Analyzer


app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's 100 most recent tweets or redirects to index
    # if unable to.
    tweets = helpers.get_user_timeline(screen_name, 100)
    if tweets == None:
        return redirect(url_for("index"))

    # Set the directories for positive and negative -words.txt.
    posDir = os.path.join(sys.path[0], "positive-words.txt")
    negDir = os.path.join(sys.path[0], "negative-words.txt")
    
    # Initialize the analyzer to analyzer
    analyzer = Analyzer(posDir, negDir)
    
    # Variables to hold the number of positive, negative, and neutral
    # tweets made by the user.
    pos = 0
    neg = 0
    neu = 0

    # Counts the number of positive, negative, and neutral tweets
    # made by the user.
    for tweet in tweets:
        tweetScore = analyzer.analyze(tweet)
        if tweetScore > 0:
            pos += 1
        elif tweetScore < 0:
            neg += 1
        else:
            neu += 1

    # generate chart
    chart = helpers.chart(pos, neg, neu)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)



"""
NOTES (CAN IGNORE)

If you want to use a method from a class from a different file, don't
forget to initialize it first! Done by calling the class name followed
by any args that the class' init function requires. Eg: 

instance = Class(arg1, arg2)
"""