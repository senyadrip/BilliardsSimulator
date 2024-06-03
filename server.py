# **
#  * @file server.py
#  * @brief Python file used for a REST API which handles client and server requests
#  *
#  **


from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qsl
import urllib.parse
import sys
import os
import phylib
import Physics
import math
import random

table_id = 0



class MyHandler(BaseHTTPRequestHandler):

    # Serve a GET Request
    def do_GET(self):

        # Check if the request is for the "index.html" page
        if self.path == "/index.html":
            with open('.' + self.path) as fp:
                content = fp.read()
            # Send the HTTP response with the HTML content
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

        # Check if the request is for Javascript
        elif self.path == "/game.js":
            with open('.' + self.path) as fp:
                content = fp.read()
            self.send_response(200)
            self.send_header("Content-type", "application/javascript")  # Set content type to JavaScript
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

        # Send a 404 if request can't be responded towards
        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(bytes("404: %s not found" % self.path, "utf-8"))

    # Serve a POST Request
    def do_POST(self):

        global table_id

        # Check if we are serving for /start
        if self.path == "/start":
            content_len = int(self.headers['Content-length'])
            post_data = self.rfile.read(content_len)
            form = urllib.parse.parse_qs(post_data.decode('utf-8'))

            # Store player names from form
            player1name = form['player1name'][0]
            player2name = form['player2name'][0]
            gameName = 'game'
            db = Physics.Database()

            # Set up Table and add balls
            game = Physics.Game(None, gameName, player1name, player2name)
            table = Physics.Table()
            def nudge():
                return random.uniform( -1.5, 1.5 );
            # 1 ball
            pos = Physics.Coordinate( 
                    Physics.TABLE_WIDTH / 2.0,
                    Physics.TABLE_WIDTH / 2.0,
                    );
            # add ball to table
            sb = Physics.StillBall( 1, pos );
            table += sb;
            # 2 ball
            pos = Physics.Coordinate(
                            Physics.TABLE_WIDTH/2.0 - (Physics.BALL_DIAMETER+4.0)/2.0 +
                            nudge(),
                            Physics.TABLE_WIDTH/2.0 - 
                            math.sqrt(3.0)/2.0*(Physics.BALL_DIAMETER+4.0) +
                            nudge()
                            );
            # add ball to table
            sb = Physics.StillBall( 2, pos );
            table += sb;
            # 3 ball
            pos = Physics.Coordinate(
                            Physics.TABLE_WIDTH/2.0 + (Physics.BALL_DIAMETER+4.0)/2.0 +
                            nudge(),
                            Physics.TABLE_WIDTH/2.0 - 
                            math.sqrt(3.0)/2.0*(Physics.BALL_DIAMETER+4.0) +
                            nudge()
                            );
            # add ball to table
            sb = Physics.StillBall( 3, pos );
            table += sb;
            # add cueball
            cueBall = Physics.StillBall(0, Physics.Coordinate(phylib.PHYLIB_TABLE_WIDTH/2, Physics.TABLE_LENGTH - Physics.TABLE_WIDTH/2.0))
            table += cueBall;
            firstSvgTag = table.svg()
            table_id = db.writeTable( table );

            # Demo html to display game
            html_content = """
            <html>
                <head>
                    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.3/jquery.min.js"></script>
                    <script src="game.js"></script>
                    <title> Pool Simulator </title>
                </head>
                <body>
                    <div id="table">
                        %s
                    </div>
                    <h1>Player 1: %s </h1>
                    <h1>Player 2: %s </h1>
                </body>
            </html>
            """ % (firstSvgTag, player1name, player2name)

            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(html_content))
            self.end_headers()
            self.wfile.write(bytes(html_content, "utf-8"))

        # Check if we are serving for /shoot
        if self.path == "/shoot":
            
            content_len = int(self.headers['Content-length'])
            post_data = self.rfile.read(content_len)
            form = urllib.parse.parse_qs(post_data.decode('utf-8'))

            # Store velocities from the form
            velX = float(form.get('velX', [0])[0])
            velY = float(form.get('velY', [0])[0])

            # Access database
            db = Physics.Database()

            # Store the table from the database
            table = db.readTable( table_id )

            game = Physics.Game(0, None, None, None)

            stringTable = table

            # Commence physics and generate new svg string to display
            svgString, table_id = game.shoot( game.gameName, game.player1Name, stringTable, velX, velY );

            self.send_response(200)
            self.send_header("Content-type", "text/plain")
            self.send_header("Content-length", len(svgString))
            self.end_headers()
            self.wfile.write(bytes(svgString, "utf-8"))


if __name__ == "__main__":
    
    db = Physics.Database(reset=True)
    db.createDB()

    port = 8000
    # Start the HTTP server
    httpd = HTTPServer(('localhost', port), MyHandler)
    print("Server listening on port:", port)
    httpd.serve_forever()




        





