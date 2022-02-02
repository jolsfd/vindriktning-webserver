const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
body {
    font-family: Courier New,Courier,Lucida Sans Typewriter,Lucida Typewriter,monospace; 
    color: #222;
    margin: 0;
}

h1,
h2,
h3 {
    margin: 0 0 0.35em;
}
  
h1 {
    font-size: 40px;
}
  
h2 {
    font-size: 28px;
    text-align: center;
}  
p {
    margin: 0 0 1.15em;
    text-align: center;
}

.container {
    max-width: 800px;
    margin-left: auto;
    margin-right: auto;
    padding: 24px;
}
  
header {
    background-color: rgb(89, 47, 145);
    color: #FFFFFF;
    text-align: center;
}
</style>
<body>

<header id="title">
    <div class="container">
        <h1>Ikea Vindriktning</h1>
    </div>
</header>

<div class="container">
    <h2>PM 2.5: <span id="SensorValue">None</span> &#181;g/m3 (Unkalibriert)</h2>
</div>

<div class="container">
    <p>Source: <a href="https://github.com/jolsfd/vindriktning">GitHub</a></p>
</div>

<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 2000); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("SensorValue").innerHTML =
      this.responseText;
      changeColor(this.responseText);
    }
  };
  xhttp.open("GET", "sensor", true);
  xhttp.send();
}

function changeColor(value) {
    color = "";

    if (35 >= value && value >= 0) {
        color = "#008000";
    } else if (85 >= value && value >= 36) {
        color = "#FFA500";
    } else if (value >= 86) {
        color = "#FF0000";
    }

    document.getElementById("title").style.backgroundColor = color;
}
</script>
</body>
</html>
)=====";
