// Packages
var express = require( 'express' );
var path = require( 'path' );

// Express
var app = express();
var port = process.env.PORT || 3000;

// Static routing
// To "web" directory in lab folder
app.use( express.static( path.join( __dirname, 'web' ) ) );

// Start server
var server = app.listen(port, function () {
  console.log( 'application listening on port ' + port);
} );
