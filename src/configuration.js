Pebble.addEventListener('showConfiguration', function(e) {
  var color = localStorage.getItem('color');
  if (! color) { color = "5500FF"; }
  var URL = 'http://aotta.github.io/Snoopy/configuration.html?' +
      'color=' + color;
  console.log('Configuration window opened. ' + URL);
  Pebble.openURL(URL);
});

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    var dictionary = {
      "KEY_COLOR": parseInt(configuration.color, 16)
    };
    localStorage.setItem('color', configuration.color);
    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Configuration sent to Pebble successfully!");
      },
      function(e) {
        console.log("Error sending configuration info to Pebble!");
      }
    );
  }
);