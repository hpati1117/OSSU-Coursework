import os
import re
import sys
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))


##DONE##
@app.route("/articles")
def articles():
    """Look up articles for geo."""

    # check if the geo (zipcode) is valid and, if not, return error
    check = db.execute("SELECT * FROM places WHERE postal_code = :geoinfo",\
    geoinfo=request.args.get("geo"))
    if not check:
        raise RuntimeError('Error looking up articles caused by: '\
                            + str(request.args.get("geo")))
        return jsonify(['Error looking up articles caused by: '\
                            + str(request.args.get("geo"))])
    
    # return the json-ified list of dictionaries containing links and 
    # article titles
    return jsonify(lookup(request.args.get("geo")))
##DONE##

##DONE##
@app.route("/search")
def search():
    """Search for places that match query."""
    
    # gets and stores q, which is a city, state, or zip code string
    # without commas
    q = request.args.get("q").replace(",", "")
    
    # if there is a zipcode in q, store it in appropriate variable
    qZip = [zipcode for zipcode in q.split() if zipcode.isdigit()]
    if qZip:
        zipCode = qZip[0]
    else:
        zipCode = False
    
    # extract city/state if in q, and store in appropriate variable
    cityState = [citystate for citystate in q.split() if citystate.isalpha()]
    
    # create a list to hold the top 10 search results
    topMatches = []
        
    # if there are both city/state and zipcode entered, search based
    # on both
    if zipCode and cityState:

        # append a wildcard to the zipcode
        zipCode = zipCode + "%"
        
        # get a list of objects with every object that has a matching zip
        allMatches = db.execute("SELECT * FROM places WHERE postal_code \
                                 LIKE :postal_code", postal_code=zipCode)
        
        # itterate through each word and try to find it in the matches
        for nextword in cityState[1:]:
            for dictObject in allMatches:
                if nextword.lower() not in dictObject["admin_name2"].lower()\
                or nextword.lower() not in dictObject["admin_name1"].lower()\
                or nextword.lower() not in dictObject["admin_code1"].lower()\
                or nextword.lower() not in dictObject["country_code"].lower()\
                or nextword.lower() not in dictObject["place_name"].lower():
                        allMatches.remove(dictObject)
    
        # add the first 10 matches into a list
        for eachDict in allMatches[:10]:
            topMatches.append(eachDict)
        
    # else if they only input a zipcode, search based on it
    elif zipCode:
        
        # append a wildcard to the zipcode
        zipCode = zipCode + "%"
        
        # get a list of objects with every object that has a matching zip
        allMatches = db.execute("SELECT * FROM places WHERE postal_code \
                                 LIKE :postal_code", postal_code=zipCode)
        
        # add the first 10 matches into a list
        for eachDict in allMatches[:10]:
            topMatches.append(eachDict)
        
    # else if they only input a city or state, search based on it
    elif cityState:
        
        # append a wildcard to the first word
        cityState[0] = cityState[0] + "%"

        # search for the first word entered in the county, city, and state
        # fields and return a dictionary for every match
        allMatches = db.execute("SELECT * FROM places WHERE\
                                (LOWER(country_code) LIKE :word OR\
                                 LOWER(admin_name1) LIKE :word OR\
                                 LOWER(admin_name2) LIKE :word OR\
                                 LOWER(admin_code1) LIKE :word OR\
                                 LOWER(place_name) LIKE :word)",\
                                 word=cityState[0].lower())
        
        # if the user entered more than one word...
        if len(cityState) > 1:

            # remove any matches that don't include the other words in
            # country/city/state
            for nextword in cityState[1:]:
                for dictObject in allMatches:
                    if nextword.lower() not in dictObject["admin_name2"].lower()\
                    or nextword.lower() not in dictObject["admin_name1"].lower()\
                    or nextword.lower() not in dictObject["admin_code1"].lower()\
                    or nextword.lower() not in dictObject["country_code"].lower()\
                    or nextword.lower() not in dictObject["place_name"].lower():
                            allMatches.remove(dictObject)
        
        # add the first 10 results into list
        for eachDict in allMatches[:10]:
            topMatches.append(eachDict)
            
    # return the list of the top 10 matches
    return jsonify(topMatches)
##DONE##


@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)
