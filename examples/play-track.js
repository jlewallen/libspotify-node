/*
  This example loads a track from a link (URI or URL).
  
  You can give a link as the first argument on the command line, or run the
  script without arguments to fetch a predefined track.
*/
var sys = require('sys'),
    util = require('util'),
    spotify = require('../spotify'),
    account = require('../account');

var link = 'spotify:track:01gCUID7bHTcp6JzeTfpIe';
if (process.argv.length > 2) link = process.argv[2];

var session = new spotify.Session({ applicationKey: account.applicationKey });
session.on('logMessage', sys.print);

session.login(account.username, account.password, function (err) {
  if (err) return sys.error(err.stack || err);
  session.getTrackByLink(link, function(err, track) {
    if (err) {
      sys.error(err.stack || err);
    }
    else {
      session.on('musicDelivery', function(message, buffer) {
        // console.log(buffer);
      });
      session.on('playTokenLost', function() {
        console.log("playTokenLost");
      });
      session.on('endOfTrack', function() {
        console.log("endOfTrack");
        track.play(function(data) { });
      });
      track.play(function(data) { });
    }
  });
});
