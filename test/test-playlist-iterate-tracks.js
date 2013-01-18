require('./test');


createSession(function (session) {
  session.playlists.addListener('loaded', function () {
    if (this.length > 0) {
      function f () {
        this.removeAllListeners();

        for (var i = 0; i < this.length; i++) {
          assert.ok(this[i] instanceof spotify.Track);
        }

        if (this.length > 0) {
          sys.puts(sys.inspect(this[0]));
        }

        session.logout(function() { console.log('Logged out'); });
      }

      var listened = false;
      for (var i = 0; i < this.length; ++i) {
        if (!this[i].loaded) {
          listened = true;
          this[i].addListener('updated', f); 
        }
      }
      if (!listened) {
        session.logout(function() { console.log('Logged out'); });
      }
    }
  });
});
